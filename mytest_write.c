#include "stdes.h"
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[])
{
	IOBUF_FILE *f1;
	
	f1 = iobuf_open ("files/test_write.txt", 'W');
	if (f1 == NULL)
		exit (-1);

   char c[] = "zz zzzzzzzz zzzzzzzz";
    size_t length = strlen(c);

    for (size_t i = 0; i < length; i++) {
        iobuf_write(&c[i], 1, 1, f1);
    }
    iobuf_close (f1);

	return 0;
}