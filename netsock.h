#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

/* POSIX */
#include <sys/types.h>  /* For data types */
#include <sys/socket.h> /* For socket(), connect(), send(), and recv() */
#include <netdb.h> 	/* For gethostbyname() */
#include <arpa/inet.h> 	/* For inet_addr() */
#include <unistd.h> 	/* For close() */
#include <netinet/in.h> /* For sockaddr_in */


int makesock(char* host, char* port,char server);
void setupsock(int srv);
