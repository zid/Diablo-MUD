#include <string.h>
#include "parse.h"
#include "client.h"
#include "buffer.h"
#include "character.h"
#include "table.h"

static table *commands;

struct command {
	const char *name;
	void (*func)(client *);
};

static void look(struct client *c)
{
	room_look(c);
}

static void whoami(struct client *c)
{
	struct character *ch;

	ch = client_character(c);
	cprintf(c, "You are %s.\r\n", character_username(ch));
	cprintf(c, "You are currently located at: '%s'\r\n",
		character_room_name(ch));
}

static struct command command_list[] = 
{
	{"whoami", 	whoami},
	{"look", 	look},
	{NULL,		NULL}
};

void parser_init(void)
{
	int i = 0;

	commands = table_new();

	while(1)
	{
		const char *name;
		void (*func)(client *);

		name = command_list[i].name;
		func = command_list[i].func;
		
		if(!name)
			break;

		table_add(commands, name, func);
		i++;
	}
}


void parse_command(struct client *c)
{
	const char *msg = client_buffer(c);
	void (*func)(client *);

	func = table_get(commands, msg);
	if(func)
	{
		func(c);
	} else {
		cprintf(c, "\r\nWhat?\r\n");
	}
}
