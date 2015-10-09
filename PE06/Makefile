##
## Makefile 
##

CC	= gcc

CFLAGS	= -Wall -Wshadow -Werror -g

## Build rule

pe06: pe06.o answer06.o 
	$(CC) $(CFLAGS) pe06.o answer06.o -o pe06

clean:	
	rm -f pe06.o answer06.o pe06

%.o:	%.c
	$(CC) $(CFLAGS) -c $<

