#include <stdlib.h>
#include <ctype.h>
#include <fcntl.h>
#include <sys/stat.h>
#include "stdes.c"
#include <unistd.h>
#include <stdarg.h>
#include <string.h>

void test_iobuf_fscanfAndfprintf(){
    IOBUF_FILE *file = iobuf_open("files/test_file1.txt", 'W');
    // Écriture de données dans le fichier avec fprintf
    int num1 = 42;
    char str1[] = "Hello";
    char ch1 = '!';
    
    //Données écrites dans le fichier
    iobuf_fprintf(file, "%d %s %c\n", num1, str1, ch1);

    // Fermeture du fichier
    iobuf_close(file);
    
    file = iobuf_open("files/test_file1.txt", 'R');
    char chaine [0];
    int num;
    char one;
    iobuf_fscanf(file, "Test fprintf: %d %s %c", &num, chaine, &one);
    iobuf_printf("Résultats de la lecture fscanf: %s %d %c\n", chaine, num, one);
}

int main()
{
    test_iobuf_fscanfAndfprintf();
    return 0;
}