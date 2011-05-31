
typedef struct {
	int socket;
	/* private stuff */
} sockinfo;


void socket_close(int s);
int socket_listen_new(int port);
sockinfo *socket_accept(int s);
