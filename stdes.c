#include <stdlib.h>
#include <ctype.h>
#include <fcntl.h>
#include <sys/stat.h>
#include "stdes.h"
#include <unistd.h>
#include <stdarg.h>

#define SIZE_OF_BUFFER 4000

typedef struct _ES_FICHIER
{
    int fd;
    int current_position, end_index;
    char mode;
    char BUFFER[SIZE_OF_BUFFER];
} IOBUF_FILE;

IOBUF_FILE *stdout = NULL;
IOBUF_FILE *stderr = NULL;

void initialize_standard_streams()
{
    stdout = malloc(sizeof(IOBUF_FILE));

    stdout->fd = 1;
    stdout->mode = 'W';
    stdout->current_position = 0;
    stdout->end_index = 0;

    stderr = malloc(sizeof(IOBUF_FILE));
    stderr->fd = 2;
    stderr->mode = 'W';
    stderr->current_position = 0;
    stderr->end_index = 0;
}

void cleanup_standard_streams()
{
    free(stdout);
    free(stderr);
}

int iobuf_flush(IOBUF_FILE *f)
{
    if (f != NULL)
    {
        int result = write(f->fd, f->BUFFER, f->current_position);
        if (result > 0)
        {
            f->current_position = 0;
            f->end_index = 0;
            return 0;
        }
        else
        {
            return -1;
        }
    }
}

IOBUF_FILE *iobuf_open(const char *nom, char mode)
{
    if (mode != 'R' && mode != 'W')
    {
        return NULL;
    }

    IOBUF_FILE *f = malloc(sizeof(IOBUF_FILE));
    if (f == NULL)
    {
        return NULL;
    }

    f->mode = mode;
    f->end_index = 0;

    if (mode == 'W')
    {
        f->fd = open(nom, O_WRONLY | O_CREAT | O_APPEND);
        f->current_position = 0;
    }
    else
    {
        f->fd = open(nom, O_RDONLY);
        f->current_position = -1;
    }

    if (f->fd == -1)
    {
        free(f);
        return NULL;
    }

    return f;
}

int iobuf_close(IOBUF_FILE *f)
{

    if (f == NULL)
    {
        return -1;
    }

    int result;
    if (f->mode == 'W' && f->current_position != f->end_index)
    {
        result = write(f->fd, f->BUFFER, f->current_position);
        if (result == -1)
        {
            return 0;
        }
    }

    result = close(f->fd);

    if (result == 0)
    {
        free(f);
        f = NULL;
    }

    return result;
}

int iobuf_write(const void *p, unsigned int taille, unsigned int nbelem, IOBUF_FILE *f)
{
    if (p == NULL || taille == 0 || nbelem == 0 || f == NULL || f->mode != 'W')
    {
        return 0;
    }

    int size_to_write = taille * nbelem;
    int size_written = 0;

    if (size_to_write > SIZE_OF_BUFFER)
    {
        size_written = write(f->fd, p, size_to_write);
        if (size_written == -1)
        {
            return 0;
        }
    }
    else
    {
        while (size_to_write > 0)
        {
            f->BUFFER[f->current_position] = *((char *)p + size_written);
            f->current_position++;
            size_written++;
            size_to_write--;

            if (f->current_position == SIZE_OF_BUFFER)
            {
                int result = iobuf_flush(f);
                if (result == -1)
                {
                    return 0;
                }
            }
        }
    }

    return (int)size_written / taille;
}

int iobuf_read(void *p, unsigned int taille, unsigned int nbelem, IOBUF_FILE *f)
{

    if (p == NULL || taille == 0 || nbelem == 0 || f == NULL || f->mode != 'R')
    {
        return 0;
    }

    int size_to_read = taille * nbelem;
    int size_read = 0;
    char temp_buff[SIZE_OF_BUFFER];
    int temp_buff_pointer = 0;

    if (size_to_read > SIZE_OF_BUFFER)
    {
        size_read = read(f->fd, p, size_to_read);
        if (size_read <= 0)
        {
            return 0;
        }
    }
    else
    {
        if (f->current_position == -1)
        {
            int result = read(f->fd, f->BUFFER, SIZE_OF_BUFFER);
            if (result <= 0)
            {
                return 0;
            }
            f->current_position = 0;
            f->end_index = result;
        }

        while (size_to_read > 0 && f->current_position < f->end_index)
        {

            temp_buff[temp_buff_pointer] = (char)f->BUFFER[f->current_position];
            f->current_position++;
            temp_buff_pointer++;
            size_read++;
            size_to_read--;

            if (f->current_position == SIZE_OF_BUFFER)
            {
                int result = read(f->fd, f->BUFFER, SIZE_OF_BUFFER);
                if (result == -1)
                {
                    return 0;
                }

                f->current_position = 0;
                f->end_index = result;
            }
        }

        if (temp_buff_pointer > 0 && (int)size_read / taille > 0)
        {
            for (unsigned int i = 0; i < size_read; i++)
            {
                *((char *)p + i) = (char)temp_buff[i];
            }
        }
    }

    return (int)size_read / taille;
}

void intToString(int num, char *nums_as_string)
{
    int i = 0;
    int isNegative = 0;

    if (num < 0)
    {
        isNegative = 1;
        num = -num;
    }

    do
    {
        nums_as_string[i++] = num % 10 + '0';
        num /= 10;
    } while (num > 0);

    if (isNegative)
    {
        nums_as_string[i++] = '-';
    }

    for (int j = 0; j < i / 2; j++)
    {
        char temp = nums_as_string[j];
        nums_as_string[j] = nums_as_string[i - j - 1];
        nums_as_string[i - j - 1] = temp;
    }

    nums_as_string[i] = '\0';
}

int iobuf_fprintf(IOBUF_FILE *f, const char *format, ...)
{
    if (f == NULL || format == NULL || f->mode != 'W')
    {
        return 0;
    }

    va_list args;
    va_start(args, format);
    int written_chars = 0;

    while (*format)
    {
        if (*format == '%')
        {
            format++;
            switch (*format)
            {
            case 'c':
            {
                char c = va_arg(args, int);
                written_chars += iobuf_write(&c, sizeof(char), 1, f);
                break;
            }
            case 's':
            {
                const char *str = va_arg(args, const char *);
                while (*str != '\0')
                {
                    written_chars += iobuf_write(str, sizeof(char), 1, f);
                    str++;
                }
                break;
            }
            case 'd':
            {
                int num = va_arg(args, int);

                char nums_as_string[12];
                intToString(num, nums_as_string);

                for (int i = 0; nums_as_string[i] != '\0'; i++)
                {
                    written_chars += iobuf_write(&nums_as_string[i], sizeof(char), 1, f);
                }
                break;
            }
            }
        }
        else
        {
            char regular_char = *format;
            written_chars += iobuf_write(&regular_char, sizeof(char), 1, f);
        }
        format++;
    }

    int result = iobuf_flush(f);
    if (result == -1)
    {
        return 0;
    }

    va_end(args);
    return written_chars;
}

int iobuf_printf(const char *format, ...)
{
    if (stdout == NULL)
    {
        initialize_standard_streams();
    }

    va_list args;
    va_start(args, format);
    int written_chars = 0;

    while (*format)
    {
        if (*format == '%')
        {
            format++;
            switch (*format)
            {
            case 'c':
            {
                char c = va_arg(args, int);
                written_chars += iobuf_write(&c, sizeof(char), 1, stdout);
                break;
            }
            case 's':
            {
                const char *str = va_arg(args, const char *);
                while (*str != '\0')
                {
                    written_chars += iobuf_write(str, sizeof(char), 1, stdout);
                    str++;
                }
                break;
            }
            case 'd':
            {
                int num = va_arg(args, int);

                char nums_as_string[12];
                intToString(num, nums_as_string);

                for (int i = 0; nums_as_string[i] != '\0'; i++)
                {
                    written_chars += iobuf_write(&nums_as_string[i], sizeof(char), 1, stdout);
                }
                break;
            }
            }
        }
        else
        {
            char regular_char = *format;
            written_chars += iobuf_write(&regular_char, sizeof(char), 1, stdout);
        }
        format++;
    }

    int result = iobuf_flush(stdout);
    if (result == -1)
    {
        return 0;
    }

    va_end(args);
    return written_chars;
}

int iobuf_fscanf(IOBUF_FILE *f, const char *format, ...)
{
    if (f == NULL || format == NULL || f->mode != 'R')
    {
        return -1;
    }

    va_list args;
    va_start(args, format);
    int read_chars = 0;

    while (*format)
    {
        if (*format == '%')
        {
            format++;
            switch (*format)
            {
            case 'c':
            {
                char *c = va_arg(args, char *);
                if (iobuf_read(c, 1, 1, f) != 1)
                {
                    va_end(args);
                    return -1;
                }
                read_chars++;
                break;
            }
            case 's':
            {
                char *str = va_arg(args, char *);
                int i = 0;
                char c;
                while (iobuf_read(&c, sizeof(char), 1, f) == 1)
                {
                    read_chars++;
                    if (c != ' ') {
                        *str = c;
                        str++;
                        break;
                    }
                }
                while (iobuf_read(&c, sizeof(char), 1, f) == 1 && c != ' ')
                {
                    *str = c;
                    str++;
                    i++;
                    read_chars++;
                }
                *str = '\0';
                break;
            }
            case 'd':
            {
                int *num = va_arg(args, int *);
                char c;
                int foundNum = 0;

                while (iobuf_read(&c, sizeof(char), 1, f) == 1)
                {
                    read_chars++;
                    if (c >= '0' && c <= '9') {
                        *num = (*num * 10) + (c - '0');
                        if (foundNum == 0) {
                            foundNum = 1;
                        }
                    } else if (foundNum == 1) {
                        break;
                    }
                }
            }
            }
        }
        format++;
    }

    va_end(args);
    return read_chars;
}

