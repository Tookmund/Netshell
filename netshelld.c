/*
 * A simple Telnet clone designed for easy remote access
 * By Jacob Adams <tookmund@gmail.com>
 */

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

#define DEBUG 		/* Enable to print debug messages, can be enabled at
			 * compile time via -DDEBUG
			 */
#define ADDR 12 	/* Length of an IPv4 address plus null */
#define SHELL "/bin/sh" /* Program to use as shell */
#define PORT "8888"	/* The port netshelld will listen on */

#ifdef DEBUG
#define DBG(string) printf(string)
#endif

/* Function run by child proccesses to network the shell */
void child(int sfd) {
	dup2(sfd,0);
	dup2(sfd,1);
	dup2(sfd,2);
	char* args[] = {SHELL};
	execv(SHELL,args);
}

int main (void) {
	int srv;
	struct addrinfo hints;
	struct addrinfo *addr;
	memset(&hints,0,sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_ADDRCONFIG | AI_PASSIVE;
	int gai = getaddrinfo(NULL,PORT,&hints,&addr);
	if (gai != 0) {
		printf("getaddrinfo error\n");
		printf(gai_strerror(gai));
	}
	/* borrow socket check from libsocket */
	struct addrinfo *check;
	for (check = addr; check != NULL; check = check->ai_next) {
		printf("Trying %s\n",check->ai_addr->sa_data);
		srv = socket(check->ai_family,check->ai_socktype,check->ai_protocol);
		if (srv < 0) continue;
		if (-1 != bind(srv,check->ai_addr,check->ai_addrlen)) {
			printf("Listening on %s\n",check->ai_addr->sa_data);
			const int       optVal = 1;
			const socklen_t optLen = sizeof(optVal);
			int rtn = setsockopt(srv, SOL_SOCKET, SO_REUSEADDR, (void*) &optVal, optLen);
			if (rtn != 0) perror("setsockopt on srv");
			break;
		}
	}
	freeaddrinfo(addr);
	listen(srv,10);
	for (;;) {
		DBG("Waiting...\n");
		int sfd = accept(srv,NULL,NULL);
		DBG("\nForking\n");
		int pid = fork();
		if (pid == -1) {
			perror("Fork");
		}
		else if (pid == 0) {
			child(sfd);
			close(sfd);
			exit(0);
		}
		else {
			close(sfd);
			DBG("\nConnected\n");
		}
	}
	return 0;
}

