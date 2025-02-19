#include "stdes.h"
#include <stdlib.h>

int main(int argc, char *argv[])
{
	IOBUF_FILE *f1;
	
	f1 = iobuf_open ("files/test_read.txt", 'R');
	if (f1 == NULL)
		exit (-1);

    char c[100];

    while (iobuf_read(&c, 8, 1, f1) == 1) {
        for (int i = 0; i < 8; i++) {
            iobuf_printf("%c\n", c[i]);
        }
    }

    iobuf_printf("%s\n", c);
    
    iobuf_close (f1);

	return 0;
}