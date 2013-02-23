#ifndef _CHAR_H
#define _CHAR_H

typedef struct character character;

character *character_init(void *);
void character_set_username(character *, char const *username);
void character_free(character *);
/* Get the username of a character */
const char *character_username(character *);
const char *character_room_name(character *);
#endif

