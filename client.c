#include <stdlib.h>
#include "client.h"
#include "socket.h"

struct client {
	char buf[256];
	int filled;
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
	clients[s]->buf[0] = 0;
	clients[s]->filled = 0;

	/* Returns the file descriptor of the client so that
	 * the server can add it to its fd list to select on.
	 */
	clientsock = socket_get(i);

	return clientsock;
}

static void client_destroy(int s)
{

}

/* s is the file descriptor of a client that has
 * pending data. Turn it back into a struct client
 * using the lookup table.
 * Read data up to 256 characters from the socket.
 */
void client_handle(int s)
{
	client *c;
	int r;

	c = clients[s];
	
	/* Read from the socket up to an amount that
	 * would not go longer than 256 bytes.
	 */
	r = socket_read(s, &c->buf[c->filled], 256 - c->filled);
	if(r == 0)
		client_destroy(s);
	
	c->filled += r;

	/* The buffer got filled entirely, cut the input short here */
	if(c->filled == 256)
	{
		c->buf[255] = '\n';
	}
	
	/* The input steam has a newline in it, we're done */
	if(c->buf[c->filled - 1] == '\n')
	{
		/* parse(c); */
	}

}

