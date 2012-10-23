#ifndef HASH_H
#define HASH_H
typedef struct table table;
table *table_new(void);
void table_add(table *, const char *, void *);
void *table_get(table *, const char *);
#endif
