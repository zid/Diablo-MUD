#ifndef _CHAR_H
#define _CHAR_H

typedef struct character character;

character *character_init();
void character_set_username(character *ch, char const *username);
void character_free(character *ch);
#endif

