#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "rooms.h"
#include "character.h"

struct character {
	char *username;
	room *r;
};

character *character_init()
{
	character *ch;
	ch = malloc(sizeof(character));
	ch->r = room_get("plaza");
	return ch;
}

void character_set_username(character *ch, char const *username) 
{
	ch->username = strdup(username);		
}


void character_free(character *ch)
{
	free(ch->username);
	free(ch);
}

