#ifndef SOCKET_H
#define SOCKET_H
typedef struct sockinfo sockinfo;

void socket_close(int s);
void socket_free(struct sockinfo *);
int socket_listen_new(int port);
sockinfo *socket_accept(int s);
int socket_get(sockinfo *);
int socket_read(int, char *, int);
int socket_send(struct sockinfo *, const char *, int);
#endif
