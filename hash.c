#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hash.h"

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

struct table_iterator {
	struct table *t;
	struct bucket *b;
	int n;
};

table_iterator *table_iterate_over(table *t)
{
	table_iterator *ti;

	ti = malloc(sizeof(struct table_iterator));
	ti->t = t;
	ti->b = t->buckets[0];
	ti->n = 0;

	return ti;
}

const char *table_iterate(table_iterator *ti)
{
	struct bucket *b;
	b = ti->b;
	if(b)
	{
		ti->b = ti->b->next;
		return b->payload;
	}

	/* We're at the end of a bucket,
	 * find a bucket with something in.
	 */
	while(1)
	{
		ti->n++;
		if(ti->n >= ti->t->nbuckets)
		{
			/* We've searched every bucket! */
			return NULL;
		}
		b = ti->t->buckets[ti->n];
		if(b)
		{
			/* This bucket points to something
			 * return it and point to the next
			 * pointer for the next call.
			 */
			ti->b = b->next;
			return b->payload;
		}
	}
}

static unsigned long sdbm_hash(const char *key)
{
	unsigned long hash = 0;
	int c;

	while((c = *key++))
		hash = c + (hash << 6) + (hash << 16) - hash;
	
	return hash;
}

struct table *table_new(void)
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

void *table_get(struct table *t, const char *key)
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

