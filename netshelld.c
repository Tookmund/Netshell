/*
 * A simple Telnet clone designed for easy remote access
 * Only supports IPv4 at them moment
 *
 * By Jacob Adams <tookmund@gmail.com>
 */

#define DEBUG /* Enable to print debug messages */
#define ADDR 12 /* Length of an IPv4 address plus null */
#define MAX 1024 /* Largest message that can be recived */
#define SHELL "/bin/sh" /* Program to use as shell */

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
#include <limits.h>

#ifdef DEBUG
#define DBG(string) printf(string)
#endif

void child(int sfd) {
	dup2(sfd,0);
	dup2(sfd,1);
	char* args[] = {SHELL};
	execv(SHELL,args);
}

int main (void) {
	/*
	struct protoent *pro;
	pro = getprotobyname("tcp");
	int protocol;
	if (pro == NULL) protocol = 0;
	else protocol = pro->p_proto;
	/* Create an socket to listen on 
	int srv;
	/* int srv = socket(AF_INET,SOCK_STREAM,protocol); 
	if (srv == -1) perror("Socket");
	*/
	int srv;
	struct addrinfo hints;
	struct addrinfo addr;
	memset(&hints,0,sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_ADDRCONFIG;
	struct addrinfo *ptr = &addr;
	int gai = getaddrinfo("localhost","telnet",&hints,&ptr);
	if (gai != 0) DBG(gai_strerror(gai));
	/* borrow socket check from libsocket */
	struct addrinfo *check;
	for (check = ptr; check != NULL; check = check->ai_next) {
		srv = socket(check->ai_family,check->ai_socktype,check->ai_protocol);
		if (srv < 0) continue;
		if (-1 != connect(srv,check->ai_addr,check->ai_addrlen)) break;
	}
	listen(srv,10);
	for (;;) {
		DBG("\nWaiting...\n");
		int sfd = accept(srv,NULL,NULL);
		DBG("\nForking\n");
		int pid = fork();
		if (pid == -1) {
			perror("Fork");
		}
		else if (pid == 0) {
			child(sfd);
			exit(0);
		}
		else {
			close(sfd);
			DBG("\nConnected\n");
		}
	}
	return 0;
}

