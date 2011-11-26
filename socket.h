#ifndef SOCKET_H
#define SOCKET_H
typedef struct sockinfo sockinfo;

void socket_close(int s);
int socket_listen_new(int port);
sockinfo *socket_accept(int s);
int socket_get(sockinfo *);
#endif
