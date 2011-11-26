#ifndef CLIENT_H
#define CLIENT_H

typedef struct client client;

void client_new(client **c, int s);
void client_handle(int s);
#endif
