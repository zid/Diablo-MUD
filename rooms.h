#ifndef ROOM_H
#define ROOM_H

typedef struct room room;

#include "client.h"
#include "character.h"

enum {
	ROOM_LOGIN,
	ROOM_LOGOUT
};

room *room_get(const char *);
void rooms_init(void);
const char *room_name(struct room *);
void room_add_character(struct room *, struct character *, int);
void room_del_character(struct room *, struct character *, int);
void room_look(struct client *c);
#endif
