#include "client.h"
#include "socket.h"

struct client{
	int a, b, c;
	struct client *next;
};

void client_new(client **c, int s)
{
	sockinfo *i;

	i = socket_accept(s);
}
