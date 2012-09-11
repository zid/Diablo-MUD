#include <stdio.h>
#include <stdlib.h>
#include "character.h"

struct character {
	char *username;
};

character *character_new(char *username) {
	character *ch;
	ch = (character *) malloc(sizeof(character));
	ch->username = username;
	
	return ch;
}

int character_set_username(charater *ch, char *username) {
	// Check whether username is validly formatted? [if not, return false?]
	
	ch->username = username;		
	return 1;
}


void character_free(character *ch) {
	free(ch->username);
	free(ch);
}

