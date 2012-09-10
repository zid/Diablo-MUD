#include "socket.h"
#include "login.h"

void login_send_banner(int cfd)
{
	const char msg[] = "Diablo MUD\r\n***\r\n";
	int msg_len = sizeof(msg) - 1;

	socket_send(cfd, msg, msg_len); /* TODO: kill the client on an error */
}

void login_send_motd(int cfd)
{
	const char msg[] = "\r\nWelcome back, $USERNAME\r\n";
	int msg_len = sizeof(msg) - 1;

	/* TODO: substitute username */
	socket_send(cfd, msg, msg_len); /* TODO: kill the client on an error */
}
