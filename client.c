#include <stdlib.h>
#include "client.h"
#include "socket.h"

struct client {
	int a, b, c;
	struct sockinfo *si;
};

static struct client **clients;
static int nclients;
static int client_list_size;

int client_new(int s)
{
	sockinfo *i;
	int clientsock;

	i = socket_accept(s);
	if(!i)
		return 0;

	nclients++;
	if(nclients > client_list_size)
	{
		client_list_size = nclients * 2;
		clients = realloc(clients, 
			sizeof(struct client *) * client_list_size);
	}
	
	clients[s] = malloc(sizeof(struct client));
	clients[s]->si = i;

	/* Returns the file descriptor of the client so that
	 * the server can add it to its fd list to select on.
	 */
	clientsock = socket_get(i);

	return clientsock;
}

/* s is the file descriptor of a client that has
 * pending data. Turn it back into a struct client
 * using the lookup table.
 */
void client_handle(int s)
{
	client *c;

	c = clients[s];
}

