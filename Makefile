CFLAGS = -ggdb -W -Wall
OBJECTS := $(patsubst %.c,%.o,$(wildcard *.c))

mud: $(OBJECTS)
	gcc -o mud $(OBJECTS) $(CFLAGS)

clean:
	@rm *.o mud
