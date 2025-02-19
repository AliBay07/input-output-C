#include <unistd.h>
#include <stdlib.h>
#include "stdes.h"

int main(int argc, char **argv)
{
    IOBUF_FILE *f1;

    f1 = iobuf_open ("files/test_write.txt", 'W');
    if (f1 == NULL) {
        exit(-1);
    }

    iobuf_fprintf(f1, " %c %s 12\n", 'a', "bonjour");
    iobuf_fprintf(f1, " %d \n", -1257);

    iobuf_close(f1);
}