#ifndef ROOM_H
#define ROOM_H

typedef struct room room;

#include "character.h"

enum {
	ROOM_LOGIN
};

room *room_get(const char *);
void rooms_init(void);
const char *room_name(struct room *);
void room_add_character(struct room *, character *, int);
#endif
