#ifndef BUFFER_H
#define BUFFER_H
typedef struct buffer buffer;

struct buffer *buffer_init(void);
int buffer_add(struct buffer *, char *, int);
const char *buffer_get(struct buffer *);
void buffer_free(struct buffer *);
#endif
