#include "parse.h"
#include "client.h"
#include "buffer.h"
#include "character.h"

static void look(struct client *c)
{
	c;
}

static void whoami(struct client *c)
{
	struct character *ch;

	ch = client_character(c);
	cprintf(c, "You are %s.\r\n", character_username(ch));
	cprintf(c, "You are currently located at: '%s'\r\n",
		character_room_name(ch));
}

void parse_command(struct client *c)
{
	const char *msg = client_buffer(c);
	int r;

	r = strcmp(msg, "whoami");
	if(r == 0)
	{
		whoami(c);
	} else {
		cprintf(c, "What?\r\n");
	}

	return;
}
