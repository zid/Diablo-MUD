#include "client.h"
#include "character.h"
#include "login.h"

void login_client(client *c)
{
	/* TODO: Check login credentials! */

	character_login(c);
}

void login_send_banner(client *c)
{
	const char msg[] = "Diablo MUD\r\n***\r\n";
	int msg_len = sizeof(msg) - 1;

	client_send(c, msg, msg_len); /* TODO: kill the client on an error */
}

void login_send_motd(client *c)
{
	const char msg[] = "\r\nWelcome back, $USERNAME\r\n";
	int msg_len = sizeof(msg) - 1;

	/* TODO: substitute username */
	client_send(c, msg, msg_len); /* TODO: kill the client on an error */
}
