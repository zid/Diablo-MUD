#include <stdlib.h>
#include <string.h>
#include "buffer.h"

#define BUFMAX 256

struct buffer {
	char *b;
	int len;
	int full;
};

static char *strnchrs(const char *buffer, int count, const char *chars)
{
	int len = strlen(chars);
	int i;
	char *b = (char *)buffer;

	while(count--)
	{
		for(i = 0; i < len; i++)
			if(*b == chars[i])
				return b;
		b++;
	}

	return NULL;
}

struct buffer *buffer_init(void)
{
	struct buffer *b;

	b = malloc(sizeof(struct buffer));
	b->b = malloc(256);
	b->len = 0;
	b->full = 0;
	return b;
}

int buffer_add(struct buffer *buffer, char *buf, int r)
{
	char *t;

	/* The last time we returned, we returned a full line
	 * start with a fresh buffer this time.
	 */
	if(buffer->full)
	{
		buffer->len = 0;
		buffer->full = 0;
	}

	/* Strip \r and \n from clients that send them */
	if((t = strnchrs(buf, r, "\r\n")))
	{
		*t = '\0';
		buffer->full = 1;
	}
	
	if(r + buffer->len >= 256)
	{
		/* Buffer too long to hold this new data
		 * so we have to cut it short instead.
		 */
		r = 255-buffer->len;
	}
	
	memcpy(&buffer->b[buffer->len], buf, r);
	buffer->b[buffer->len + r] = '\0';

	if((t = strchr(buffer->b, '\n')))
		buffer->full = 1;

	buffer->len += r;

	return buffer->full;
}

const char *buffer_get(struct buffer *b)
{
	return b->b;
}

void buffer_free(struct buffer *b)
{
	free(b->b);
	free(b);
}
