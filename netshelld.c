/*
 * A simple Telnet clone designed for easy remote access
 * Socket handling based on Practical Sockets (https://github.com/Tookmund/PracticalSockets/)
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

/* Make an IPv4 TCP socket holder */
int sockDesc;

/* Borrow some functions from Practical Sockets */

static void fillAddr(char* address, unsigned short port,
	struct sockaddr_in addr) {
	memset(&addr, 0, sizeof(addr)); /* Zero out address structure */
	addr.sin_family = AF_INET; /* Internet address */
	struct hostent *host; /* Resolve name */
	if ((host = gethostbyname(address)) == NULL) {
		/*
		 * strerror() will not work for gethostbyname() and hstrerror()
		 * is supposedly obsolete
		 */
		DBG("\nFailed to resolve name (gethostbyname())\n");
	}
	addr.sin_addr.s_addr = *((unsigned long *) host->h_addr_list[0]);
	addr.sin_port = htons(port); /* Assign port in network byte order */
}

void setLocalAddressAndPort(char* localAddress,
	unsigned short localPort) {
	/* Get the address of the requested host */
	struct sockaddr_in localAddr;
	fillAddr(localAddress, localPort, localAddr);
	if (bind(sockDesc, (struct sockaddr *) &localAddr, sizeof(struct sockaddr_in)) < 0) {
		perror("\nSet of local address and port failed (bind())");
	}
}
void child(int sfd) {
	dup2(sfd,0);
	dup2(sfd,1);
	char* args[] = {SHELL};
	execv(SHELL,args);
}

int main (void) {
	struct protoent *pro;
	pro = getprotobyname("tcp");
	int protocol;
	if (pro == NULL) protocol = 0;
	else protocol = pro->p_proto;
	sockDesc = socket(AF_INET,SOCK_STREAM,protocol);
	setLocalAddressAndPort("localhost",23);
	listen(sockDesc,10);
	for (;;) {
		DBG("\nWaiting...\n");
		int sfd = accept(sockDesc,NULL,NULL);
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
}
