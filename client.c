#include <stdlib.h>
#include <string.h>
#include "client.h"
#include "socket.h"
#include "buffer.h"

struct client {
	struct buffer *buffer;
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
	clients[newfd]->buffer = buffer_init();
	/* The server wants this fd so it can update the
	 * file descriptor read set.
	 */
	return newfd;
}

static void client_destroy(int s)
{
	client *c;

	c = clients[s];	
	clients[s] = NULL;
	
	socket_free(c->si);
	buffer_free(c->buffer);
	free(c);
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
	char buf[256];
	int r;

	c = clients[s];
	
	/* Read up to 256 bytes from the client */
	r = socket_read(s, buf, 256);
	if(r == 0)
	{
		/* 0 bytes read means the client has disconnected */
		client_destroy(s);
		return 0;
	}

	/* Returns whether the buffer has something useful in it */
	r = buffer_add(c->buffer, buf, r); 
	if(r)
	{
		printf("Client said %s", buffer_get(c->buffer));
	//	parse();

	}
	return 1;
}

