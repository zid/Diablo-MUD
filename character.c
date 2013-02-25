#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "rooms.h"
#include "character.h"
#include "client.h"

struct character {
	char *username;
	struct client *c;
	room *r;
};

void character_prompt(character *ch)
{
	client_prompt(ch->c);
}

void chprintf(character *ch, const char *fmt, ...)
{
	va_list ap;

	va_start(ap, fmt);
	vcprintf(ch->c, fmt, ap);
	va_end(ap);
}

void character_login(client *c)
{
	character *ch;

	ch = client_character(c);
	room_add_character(ch->r, ch, ROOM_LOGIN);
}

room *character_room(character *ch)
{
	return ch->r;
}

character *character_init(client *c)
{
	character *ch;
	ch = malloc(sizeof(character));

	/* For now, just shove everybody in the plaza when they connect */
	ch->r = room_get("plaza");
	/* Backreference to the controlling client, to assist in lookups */
	ch->c = c;

	return ch;
}

void character_set_username(character *ch, char const *username)
{
	ch->username = strdup(username);
}

const char *character_username(character *ch)
{
	return ch->username;
}

const char *character_room_name(character *ch)
{
	return room_name(ch->r);
}

void character_free(character *ch)
{
	room_del_character(ch->r, ch, ROOM_LOGOUT);
	free(ch->username);
	free(ch);
}

