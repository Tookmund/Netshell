FLAGS :=-Wall -Werror -g
CC := gcc

all: netshell netshelld

netshell: netshell.c netsock.c
	$(CC) $(FLAGS) netshell.c netsock.c -o netshell

netshelld: netshelld.c netsock.c
	$(CC) $(FLAGS) netshelld.c netsock.c -o netshelld
