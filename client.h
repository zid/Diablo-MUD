#ifndef CLIENT_H
#define CLIENT_H

typedef struct client client;

int client_init(int s);
int client_handle(int s);
void client_destroy(struct client *);
void client_send(struct client *, const char *, int);
/* Get a the input buffer from a client */
const char *client_buffer(struct client *);
/* Get a character this client controls */
struct character *client_character(struct client *c);
/* Print formatted text to a client */
void cprintf(struct client *, const char *, ...);


enum {
	CONNECTING,
	USERNAME,
	PASSWORD,
	CONNECTED,
	COMMAND
};
#endif
