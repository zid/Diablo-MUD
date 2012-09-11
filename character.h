#ifndef _CHAR_H
#define _CHAR_H

#define MAX_USERNAME_LENGTH = 10;

typedef struct character character;

character *character_new();

int character_set_username(character *ch, char *username);

void character_free(character *ch);
#endif

