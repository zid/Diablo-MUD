#ifndef CLIENT_H
#define CLIENT_H

typedef struct client client;

int client_new(int s);
int client_handle(int s);
void client_destory(client *client);

enum {
	CONNECTING,
	USERNAME,
	PASSWORD,
	CONNECTED
};
#endif
