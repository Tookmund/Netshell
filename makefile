FLAGS=-Wall -Werror

all: netshell netshelld

netshell: netshell.c netsock.c
	gcc $(FLAGS) netshell.c netsock.c -o netshell

netshelld: netshelld.c netsock.c
	gcc $(FLAGS) netshelld.c netsock.c -o netshelld
