#include "socket.h"

typedef struct {
	int a, b, c;
	struct client *next;
} client;

void client_new(client **c, int s)
{
	sockinfo *i;

	i = socket_accept(s);
}
