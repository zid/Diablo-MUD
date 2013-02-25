#ifndef CHARACTER_H
#define CHARACTER_H

typedef struct character character;

#include "rooms.h"

character *character_init(client *);
void character_set_username(character *, char const *username);
void character_free(character *);
/* Get the username of a character */
const char *character_username(character *);
const char *character_room_name(character *);
void chprintf(character *, const char *, ...);
void character_login(client *);
room *character_room(character *);
void character_prompt(character *);
#endif

