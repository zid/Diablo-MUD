#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "client.h"
#include "socket.h"
#include "buffer.h"
#include "login.h"
#include "character.h"

struct client {
	struct buffer *buffer;
	struct sockinfo *si;
	struct character *ch;
	int state;
};

static struct client **clients;
static int maxfd;

static void client_send(int cfd, const char *msg, int msg_len)
{
	int r;
	r = socket_send(cfd, msg, msg_len);
	 
	// The client has disconnected, destroy it
	if(!r)
		client_destroy(cfd);
}


static void login_ask_username(int cfd, client *c)
{
	const char msg[] = "\r\nusername: ";
	int msg_len = sizeof(msg) - 1;

	c->state = USERNAME;
	client_send(cfd, msg, msg_len); /* TODO: kill the client on an error */
}

static void login_ask_password(int cfd, client *c)
{
	const char msg[] = "\r\npassword: ";
	int msg_len = sizeof(msg) - 1;

	c->state = PASSWORD;
	client_send(cfd, msg, msg_len); /* TODO: kill the client on an error */
}

static void send_invalid_username_format(int cfd)
{
	const char msg[] = "\r\nusername must be ... ";

	int msg_len = sizeof(msg) - 1;
	client_send(cfd, msg, msg_len); /* TODO: kill the client on an error */

}

static void send_prompt(int cfd)
{
	const char msg[] = "\r\n> ";
	int msg_len = sizeof(msg) - 1;

	client_send(cfd, msg, msg_len);
}

static void handle_username(int cfd, client *c)
{
	char const *buf = buffer_get(c->buffer);
	int str_len = (int) strlen(buf);
	char *name = malloc(str_len + 1);
	memcpy(name, buf, str_len + 1);

	character *chr = c->ch;

	// Username invalidly formatted
	if(!character_set_username(chr, name))
	{
		send_invalid_username_format(cfd);
		login_ask_username(cfd, c);
	} else
	{
		login_ask_password(cfd, c);
	}

}

static void parse(int cfd, client *c)
{
	switch(c->state)
	{
		case CONNECTING:
			/* not used */
			break;
		case USERNAME:
			handle_username(cfd, c);

			break;
		case PASSWORD:
			/* TODO: check the username and password */
			login_send_motd(cfd);
			c->state = CONNECTED;
			send_prompt(cfd);
			break;
		case CONNECTED:
			/* TODO: parse the command prompt */
			send_prompt(cfd);
			break;
	}
}


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
	clients[newfd]->state = CONNECTING;
	clients[newfd]->ch = character_new();

	login_send_banner(newfd);

	login_ask_username(newfd, clients[newfd]);

	/* The server wants this fd so it can update the
	 * file descriptor read set.
	 */
	return newfd;
}

void client_destroy(int s)
{
	client *c;

	c = clients[s];	
	clients[s] = NULL;
	
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
		client_destroy(s);
		return 0;
	}

	/* Returns whether the buffer has something useful in it */
	r = buffer_add(c->buffer, buf, r); 
	if(r)
	{
	//	printf("Client %d said '%s'\n", s, buffer_get(c->buffer));
		parse(s, c);

	}
	return 1;
}


