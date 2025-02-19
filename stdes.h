#ifndef _STDES_H
#define _STDES_H

struct _ES_FICHIER;
typedef struct _ES_FICHIER IOBUF_FILE;

extern IOBUF_FILE *stdout;
extern IOBUF_FILE *stderr;

/* mode: 'L' = lecture, 'E' = écriture */
IOBUF_FILE *iobuf_open(const char *nom, char mode);
int iobuf_close(IOBUF_FILE*f);
int iobuf_read(void *p, unsigned int taille, unsigned int nbelem, IOBUF_FILE *f);
int iobuf_write(const void *p, unsigned int taille, unsigned int nbelem, IOBUF_FILE *f);
int iobuf_flush(IOBUF_FILE *f);

int iobuf_fprintf(IOBUF_FILE *f, const char *format, ...);
/* directly in stdout */
int iobuf_printf (const char *format, ...);
int iobuf_fscanf(IOBUF_FILE *f, const char *format, ...);

#endif

