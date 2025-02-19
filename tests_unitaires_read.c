#include <stdlib.h>
#include <ctype.h>
#include <fcntl.h>
#include <sys/stat.h>
#include "stdes.c"
#include <unistd.h>
#include <stdarg.h>
#include <string.h>

// La fonction de test pour iobuf_write
void test_iobuf_read(){

    // Cas de test 1: Lecture de données depuis un fichier en mode lecture ('R')
    IOBUF_FILE *file1 = iobuf_open("files/test_file1.txt", 'R');
    char buffer1[100];  // Assurez-vous que la taille du tampon est suffisante
    int result1 = iobuf_read(buffer1, sizeof(char), 99, file1);
    
    // Cas de test 2: Lecture à partir d'un fichier vide
    IOBUF_FILE *file2 = iobuf_open("files/test_file2.txt", 'R');
    char buffer2[100];  // Assurez-vous que la taille du tampon est suffisante
    int result2 = iobuf_read(buffer2, sizeof(char), 99, file2);
    //PROBLEME DE LECTURE CAR RENVOIE DES CHARACTERES �� (surement lié à l'initialisation)

    // Cas de test 3: Tentative de lecture depuis un fichier ouvert en mode écriture ('W')
    IOBUF_FILE *file3 = iobuf_open("files/test_file1.txt", 'W');
    char buffer3[100];  // Assurez-vous que la taille du tampon est suffisante
    int result3 = iobuf_read(buffer3, sizeof(char), 99, file3);
    //Probleme car ça n'empeche pas de lire mais juste des char aleatoires dans le buffer (sûrement lié à l'initialisation)

    // Cas de test 4: Lecture à partir d'un fichier après sa fermeture
    IOBUF_FILE *file4 = iobuf_open("files/test_file1.txt", 'R');
    iobuf_close(file4);
    char buffer4[100];  // Assurez-vous que la taille du tampon est suffisante
    int result4 = iobuf_read(buffer4, sizeof(char), 99, file4);
    //Probleme car toujours pas empeché de lire mais ne lit pas les données (sûrement lié au fait que l'index est à la fin et donc il n'y a rien après)



    iobuf_printf("Test 1: Result = %d, Data Read: %s\n", result1, buffer1);
    iobuf_printf("Test 2: Result = %d, Data Read: %s\n", result2, buffer2);
    iobuf_printf("Test 3: Result = %d, Data Read: %s\n", result3, buffer3);
    iobuf_printf("Test 4: Result = %d, Data Read: %s\n", result4, buffer4);


}

int main() {
    // Appel de la fonction de test
    test_iobuf_read();

    return 0;
}
