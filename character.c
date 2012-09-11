#include <stdio.h>
#include <stdlib.h>
#include "character.h"

struct character {
	char *username;
};

character *character_new()
{
	character *ch;
	ch = malloc(sizeof(character));
	
	return ch;
}

int character_set_username(character *ch, char *username) 
{
	// Check whether username is validly formatted? [if not, return false?]
	
	ch->username = username;		
	return 1;
}


void character_free(character *ch)
{
	free(ch->username);
	free(ch);
}

