#ifndef HASH_H
#define HASH_H
typedef struct table table;
typedef struct table_iterator table_iterator;
table *table_new(void);
void table_add(table *, const char *, void *);
void *table_get(table *, const char *);

table_iterator *table_iterate_over(table *);
void *table_iterate(table_iterator *t);
#endif
