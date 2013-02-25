#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "rooms.h"
#include "table.h"
#include "client.h"
#include "character.h" 

static table *rooms;

static const char *reasons[] = {
	"login"
};

struct room {
	const char *name;
	const char *desc;
	table *exits, *furniture, *chars, *objects;
};

void rooms_init(void)
{
	rooms = table_new();
}

void room_look(client *c)
{
	table_iterator *t;
	int cont;
	character *ch;
	room *r;

	ch = client_character(c);
	r = character_room(ch);

	chprintf(ch, "%s\r\n%s\r\n", r->name, r->desc);

	t = table_iterate_over(r->chars);
	cont = 0;

	while(1)
	{
		const char *name;
		character *tc;

		tc = table_iterate(t);
		if(!tc)
			break;

		/* Don't tell someone about themself */
		if(tc == ch)
			continue;

		name = character_username(tc);
		if(cont)
			chprintf(ch, ", %s", name);
		else
			chprintf(ch, "%s", name);
		cont++;
	}
	if(cont)
		chprintf(ch, " is standing here\r\n");
}

void room_add_character(struct room *r, character *ch, int reason_code)
{
	//character *t;
	table_iterator *t;

	/* Broadcast to each character in the room that a
	 * new character has entered the room.
	 */
	t = table_iterate_over(r->chars);
	while(1)
	{
		character *tc;
		const char *name, *reason;
		tc = table_iterate(t);
		if(!tc)
			break;

		name = character_username(ch);
		reason = reasons[reason_code];
		chprintf(tc, "%s entered the room via %s\r\n", 
			name, reason );
	}

	table_add(r->chars, character_username(ch), ch); 
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
			break;
			
		parse_line(r, buf);
	}

	return r;
}

const char *room_name(room *r)
{
	return r->name;
}

struct room *room_get(const char *name)
{
	FILE *f;
	char buf[128];
	struct room *r;

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
	if(!r)
	{
		printf("Fuck, couldn't parse room %s\n", name);
		return NULL;
	}
	r->name = strdup(name);
	table_add(rooms, name, r);
	fclose(f);

	return r;
}
