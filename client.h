#ifndef CLIENT_H
#define CLIENT_H

typedef struct client client;

int client_new(int s);
void client_handle(int s);
#endif
