#ifndef _CHAR_H
#define _CHAR_H

#define MAX_USERNAME_LENGTH = 10;

typedef struct character character;

character *character_init();
void character_set_username(character *ch, char const *username);
void character_free(character *ch);
#endif

