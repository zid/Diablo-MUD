#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include "client.h"
#include "socket.h"
#include "buffer.h"
#include "login.h"
#include "character.h"
#include "parse.h"

struct client {
	struct buffer *buffer;
	struct sockinfo *si;
	struct character *ch;
	int state;
};

static struct client **clients;
static int maxfd;

void client_send(client *c, const char *msg, int msg_len)
{
	int r;
	r = socket_send(c->si, msg, msg_len);
	 
	/* The client has disconnected, destroy it */
	if(!r)
		client_destroy(c);
}


static void login_ask_username(client *c)
{
	const char msg[] = "username: ";

	client_send(c, msg, sizeof(msg) - 1); 
}

static void login_ask_password(client *c)
{
	const char msg[] = "password: ";

	client_send(c, msg, sizeof(msg) - 1);
}

static void send_prompt(client *c)
{
	const char msg[] = "> ";

	client_send(c, msg, sizeof(msg) - 1);
}

static void handle_username(client *c)
{
	const char *buf;

	buf = buffer_get(c->buffer);
	character_set_username(c->ch, buf);
}

static void parse(client *c)
{
	switch(c->state)
	{
		case CONNECTING:
			login_send_banner(c);
			login_ask_username(c);
			c->state = USERNAME;
		break;
		case USERNAME:
			handle_username(c);
			login_ask_password(c);
			c->state = PASSWORD;
		break;
		case PASSWORD:
			/* TODO: check the username and password */
			login_client(c);
			c->state = CONNECTED;
			send_prompt(c);
		break;
		case CONNECTED:
			/* TODO: parse the command prompt */
			//send_prompt(c);
			parse_command(c);
			send_prompt(c);
		break;
	}
}

void cprintf(client *c, const char *fmt, ...)
{
	va_list ap;
	char buf[256];
	int len;

	va_start(ap, fmt);
	len = vsnprintf(buf, 256, fmt, ap);
	va_end(ap);

	client_send(c, buf, len); 	

}

struct character *client_character(client *c)
{
	return c->ch;
}

const char *client_buffer(client *c)
{
	return buffer_get(c->buffer);
}

int client_init(int s)
{
	sockinfo *i;
	struct client *c;
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


	c = malloc(sizeof(struct client));
	c->si = i;
	c->buffer = buffer_init();
	c->ch = character_init(c);
	c->state = CONNECTING;

	clients[newfd] = c;

	parse(c);

	/* The server wants this fd so it can update the
	 * file descriptor read set.
	 */
	return newfd;
}

void client_destroy(client *c)
{
	int fd;

	fd = socket_get(c->si);
	clients[fd] = NULL;
	
	socket_free(c->si);
	buffer_free(c->buffer);
	character_free(c->ch);
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
		client_destroy(c);
		return 0;
	}

	/* Returns whether the buffer has something useful in it */
	r = buffer_add(c->buffer, buf, r); 
	if(r)
	{
	//	printf("Client %d said '%s'\n", s, buffer_get(c->buffer));
		parse(c);

	}
	return 1;
}


