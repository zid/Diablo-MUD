#include <stdio.h>
#include <stdlib.h>
#include "character.h"
#include <string.h>

struct character {
	char *username;
};

character *character_init()
{
	character *ch;
	ch = malloc(sizeof(character));
	
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

