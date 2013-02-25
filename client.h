#ifndef CLIENT_H
#define CLIENT_H

typedef struct client client;

#include <stdarg.h>

int client_init(int s);
int client_handle(int s);
void client_destroy(struct client *);
void client_send(struct client *, const char *, int);
/* Get a the input buffer from a client */
const char *client_buffer(struct client *);
/* Get a character this client controls */
struct character *client_character(struct client *c);
/* Print formatted text to a client */
void cprintf(client *, const char *, ...);
void vcprintf(client *, const char *, va_list);
void client_prompt(client *c);
enum {
	CONNECTING,
	USERNAME,
	PASSWORD,
	CONNECTED,
	COMMAND
};
#endif
