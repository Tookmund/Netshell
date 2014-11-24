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

#define MAXBUF 1024

int main (int argc, char* argv[]) {
	if (argc < 2) printf("Usage: %s host port\n",argv[0]);
	else {
		int sfd;
		struct addrinfo hints;
		struct addrinfo *addr;
		memset(&hints,0,sizeof(hints));
		hints.ai_family = AF_UNSPEC;
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_flags = AI_ADDRCONFIG | AI_PASSIVE;
		int gai = getaddrinfo(argv[1],argv[2],&hints,&addr);
		if (gai != 0) {
			printf("getaddrinfo error\n");
			printf(gai_strerror(gai));
		}
		/* borrow socket check from libsocket */
		struct addrinfo *check;
		for (check = addr; check != NULL; check = check->ai_next) {
			printf("Trying %s\n",check->ai_addr->sa_data);
			sfd = socket(check->ai_family,check->ai_socktype,check->ai_protocol);
			if (sfd < 0) continue;
			if (-1 != connect(sfd,check->ai_addr,check->ai_addrlen)) {
				printf("Listening on %s\n",check->ai_addr->sa_data);
				const int       optVal = 1;
				const socklen_t optLen = sizeof(optVal);
				int rtn = setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, (void*) &optVal, optLen);
				if (rtn != 0) perror("setsockopt on sfd");
				break;
			}
		}
		freeaddrinfo(addr);
		char buf [MAXBUF];
		const int bufsize = (MAXBUF)-1;
		memset(&buf,0,sizeof(buf));
		int rb; /* number of bytes read */
		int wb; /* number of bytes written */
		for (;;) {
			memset(&buf,0,sizeof(buf));
			rb = 0;
			wb = 0;
			rb = read(sfd, buf,bufsize);
			if (rb > 0) write(1,buf,bufsize);
			rb = read(0,buf,bufsize);
			if (rb > 0) write(1,buf,bufsize);
		}
	}
	return 0;
}
