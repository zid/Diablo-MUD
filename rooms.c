#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hash.h"

static table *rooms;

struct room {
	const char *desc;
	table *exits, *furniture, *chars, *objects;
};

void rooms_init(void)
{
	rooms = table_new();
}

static struct room *room_new(void)
{
	struct room *r;

	r = calloc(sizeof(struct room), 1);
	
	r->exits = table_new();
	r->furniture = table_new();
	r->chars = table_new();
	r->objects = table_new();

	return r;
}

static int get_line(FILE *f, char *buf)
{
	int c, i = 0;

	while(1)
	{
		c = fgetc(f);
		if(c == EOF)
		{
			fprintf(stderr, "Malformed area file\n"); 
			break;
		}
		if(c == '\n')
			break;
		buf[i] = c;
		i++;
	}
	buf[i] = '\0';

	return i;
}

static void parse_line(struct room *r, char *buf)
{
	int type;
	char *key, *value;

	type = buf[0];
	key = buf+1;
	
	value = strchr(buf, '=');
	if(value)
		*value++ = '\0'; /* Null terminate key */
	else
		value = key;
	switch(type)
	{
		case '_':	/* Room description */
			r->desc = strdup(key);
		break;
		case '#':	/* furniture */
			table_add(r->furniture, strdup(key), strdup(value));
		break;
		case '$':
			/* TODO: Load from objects/name */
		break;
		case '!': /* NPC */
			/* TODO Load NPC */
		break;
	}
}

static struct room *room_parse(FILE *f)
{
	char buf[512];
	struct room *r;
	int n;

	r = room_new();

	while(1)
	{
		n = get_line(f, buf);
		if(!n)
			goto err;
			
		parse_line(r, buf);
	}

	return r;

err:
	/* TODO: Free the tables and strings allocated already */
	free(r);

	return NULL;
}

struct room *room_get(const char *name)
{
	FILE *f;
	char buf[128];
	struct room *r;

	/* This is here because nothing calls room_init yet, maybe main.c->mud_init()->rooms_init()? */
	if(!rooms)
		rooms_init();
	/* See if the room is already loaded */
	r = table_get(rooms, name);
	if(r)
		return r;

	/* Room wasn't already loaded - try to find it on disk */
	snprintf(buf, 128, "rooms/%s.txt", name);
	f = fopen(buf, "rb");
	if(!f)
		return NULL;	/* -ENOROOM */

	r = room_parse(f);

	fclose(f);
	return r;
}
