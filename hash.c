#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INITIAL_BUCKETS 16

struct bucket {
	const char *key;
	void *payload;
	struct bucket *next;
};

struct table {
	struct bucket **buckets;
	unsigned int nbuckets;
};

static unsigned long sdbm_hash(const char *key)
{
	unsigned long hash = 0;
	int c;

	while((c = *key++))
		hash = c + (hash << 6) + (hash << 16) - hash;
	
	return hash;
}

struct table *new_table(void)
{
	struct table *t;

	t = malloc(sizeof(struct table *));
	t->buckets = calloc(sizeof(struct bucket *), INITIAL_BUCKETS);
	t->nbuckets = INITIAL_BUCKETS;
	return t;
}

void table_add(struct table *t, const char *key, void *payload)
{
	unsigned long hash;
	unsigned int nbucket;
	struct bucket *b;

	hash = sdbm_hash(key);

	nbucket = hash % t->nbuckets;

	b = malloc(sizeof(struct bucket));
	b->payload = payload;
	b->key = strdup(key);
	b->next = t->buckets[nbucket];
	t->buckets[nbucket] = b;
}

void *table_lookup(struct table *t, const char *key)
{
	unsigned long hash;
	unsigned int bucket;
	struct bucket *b;

	hash = sdbm_hash(key);
	bucket = hash % t->nbuckets;

	for(b = t->buckets[bucket]; b; b = b->next)
	{
		if(strcmp(b->key, key) == 0)
			return b->payload;
	}

	return NULL;
}

