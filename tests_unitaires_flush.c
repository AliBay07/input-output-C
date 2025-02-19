#include <stdlib.h>
#include <ctype.h>
#include <fcntl.h>
#include <sys/stat.h>
#include "stdes.c"
#include <unistd.h>
#include <stdarg.h>
#include <string.h>

void test_iobuf_flush(){
    // Test fonctionnement du flush avec un tampon remplie
    IOBUF_FILE *file = iobuf_open("files/test_file1.txt", 'W');
    const char *data = "Test data for flushing the buffer.";
    int result_write = iobuf_write(data, sizeof(char), strlen(data), file);
    int result_flush = iobuf_flush(file);
    int result_close = iobuf_close(file);
    // Affichage des résultats
    iobuf_printf("Resultat de l'écriture : %d\n", result_write);
    iobuf_printf("Resultat du flush : %d\n", result_flush);
    iobuf_printf("Resultat de la fermeture : %d\n", result_close);


    // Test fonctionnement du flush avec un tampon vide
    IOBUF_FILE *file2 = iobuf_open("files/test_file2.txt", 'W');
    int result_flush2 = iobuf_flush(file2);
    int result_close2 = iobuf_close(file2);
    // Affichage des résultats
    iobuf_printf("Resultat du flush : %d\n", result_flush2);
    iobuf_printf("Resultat de la fermeture : %d\n", result_close2);

    // Test fcontionnement du flush après écriture partielle
    IOBUF_FILE *file3 = iobuf_open("files/test_file1.txt", 'W');
    const char *data2 = "Partial data for flushing.";
    int result_write3 = iobuf_write(data2, sizeof(char), 10, file3);
    int result_flush3 = iobuf_flush(file3);
    iobuf_printf("Resultat de l'écriture partielle : %d\n", result_write3);
    iobuf_printf("Resultat du flush : %d\n", result_flush3);
    result_write3 = iobuf_write(data2, sizeof(char), 18, file3);
    result_flush3 = iobuf_flush(file3);
    iobuf_printf("Resultat de l'écriture partielle : %d\n", result_write3);
    iobuf_printf("Resultat du flush : %d\n", result_flush3);
    int result_close3 = iobuf_close(file3);
    
    iobuf_printf("Resultat de la fermeture : %d\n", result_close3);

}

int main()
{
    test_iobuf_flush();

    return 0;
}