#include <stdlib.h>
#include <unistd.h>
#include "socket.h"
#include "client.h"
#include "server.h"

struct server {
	int socket;
	int maxfd;
	fd_set readfds;
};

server *server_new(int port)
{
	server *s;
	s = malloc(sizeof(server));

	s->socket = socket_listen_new(port);
	s->maxfd = s->socket;
	FD_ZERO(&s->readfds);
	FD_SET(s->socket, &s->readfds);

	return s;
}

int server_do(server *s)
{
	fd_set readfds;
	int i;

	/* s->readfds is an accurate list of all our
	 * clients currently connected. Copy it here
	 * and then we can act on every client.
	 */
	readfds = s->readfds;

	select(s->maxfd + 1, &readfds, NULL, NULL, NULL);

	/* Check for file descriptors that need reading
	 * and pass them off to client_handle if they do.
	 */
	for(i = 0; i <= s->maxfd; i++)
	{
		if(!FD_ISSET(i, &readfds))
			continue;

		/* The server's socket needing reading
		 * is special, it means a new client has
		 * connected so handle it seperately.
		 */
		if(i == s->socket)
		{
			int newfd;
			
			newfd = client_init(s->socket);
			if(!newfd)
				continue;
		
			FD_SET(newfd, &s->readfds);
			if(newfd > s->maxfd)
				s->maxfd = newfd;
		} else {
			int r;

			/* If r is 0, then the client disconnected. */
			r = client_handle(i);
			if(!r)
				FD_CLR(i, &s->readfds);
				
		}
	}

	return 1;
}

void server_destroy(server *s)
{
	socket_close(s->socket);
	free(s);
}
