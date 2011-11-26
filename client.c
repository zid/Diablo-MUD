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

/* s is the file descriptor of a client that has
 * pending data. Turn it back into a struct client
 * using the lookup table.
 */
void client_handle(int s)
{

}
