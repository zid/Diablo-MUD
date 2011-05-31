#include <stdio.h>
#include "server.h"

int main(void)
{
	server *s;

	printf("Starting server.\n");

	s = server_new(4242);

	while(server_do(s))
		;

	server_destroy(s);

	return 0;
}
