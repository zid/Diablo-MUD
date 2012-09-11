#ifndef _CHAR_H
#define _CHAR_H

typedef struct character character;

character *character_new(char *username);

int character_set_username(character *ch, char *username);

void character_free(character *ch);
#endif

