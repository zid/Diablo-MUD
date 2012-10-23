#ifndef ROOM_H
#define ROOM_H
typedef struct room room;
room *room_get(const char *);
void rooms_init(void);
#endif
