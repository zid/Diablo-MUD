#include <stdlib.h>
#include <string.h>
#include "client.h"
#include "socket.h"

struct client {
	char buf[256];
	int filled;
	struct sockinfo *si;
};

static struct client **clients;
static int maxfd;

int client_new(int s)
{
	sockinfo *i;
	int newfd;

	i = socket_accept(s);
	if(!i)
		return 0;

	/* We have the connection details, but not the actual
	 * file decriptor itself, use socket.c to extract it
	 * for us.
	 */
	newfd = socket_get(i);

	/* First new client - list needs creating */
	if(!clients)
	{
		clients = calloc(newfd, sizeof(struct client *));
		maxfd = newfd;
	}

	/* This fd is the largest fd we've ever seen
	 * so the client array won't be big enough for it.
	 * Reallocate it big enough for the new fd
	 * and zero all of the memory between the  
	 * previous biggest and the new biggest.
	 */
	if(newfd > maxfd)
	{
		clients = realloc(clients, 
			sizeof(struct client *) * (newfd + 1));
		memset(&clients[maxfd + 1], 0, 
			(newfd - maxfd) * sizeof(struct client *));
		maxfd = newfd;
	}
	
	clients[newfd] = malloc(sizeof(struct client));
	clients[newfd]->si = i;
	clients[newfd]->buf[0] = 0;
	clients[newfd]->filled = 0;

	/* The server wants this fd so it can update the
	 * file descriptor read set.
	 */
	return newfd;
}

static void client_destroy(int s)
{
	client *c;

	c = clients[s];
	free(c->si);
	free(c);
	clients[s] = NULL;
	socket_close(s);
}

/* s is the file descriptor of a client that has
 * pending data. Turn it back into a struct client
 * using the lookup table.
 * Read data up to 256 characters from the socket.
 * Returns the status of the client, true for alive
 * or false for dead.
 */
int client_handle(int s)
{
	client *c;
	int r;

	c = clients[s];
	
	/* Read from the socket up to an amount that
	 * would not go longer than 256 bytes.
	 */
	r = socket_read(s, &c->buf[c->filled], 256 - c->filled);
	if(r == 0)
	{
		client_destroy(s);
		return 0;
	}
	c->filled += r;

	/* The buffer got filled entirely, cut the input short here */
	if(c->filled == 256)
	{
		c->buf[255] = '\n';
	}
	
	/* The input steam has a newline in it, we're done */
	if(c->buf[c->filled - 1] == '\n')
	{
		c->buf[c->filled] = '\0'; 
		/* parse(c); */
		c->filled = 0;
		c->buf[0] = 0;
	}

	return 1;
}

