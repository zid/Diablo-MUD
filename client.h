#ifndef CLIENT_H
#define CLIENT_H

typedef struct client client;

int client_init(int s);
int client_handle(int s);
void client_destroy(int cfd);

enum {
	CONNECTING,
	USERNAME,
	PASSWORD,
	CONNECTED
};
#endif
