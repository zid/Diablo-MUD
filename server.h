#ifndef SERVER_H
#define SERVER_H
typedef struct server server;
server *server_new(int port);
int server_do(server *s);
void server_destroy(server *s);
#endif
