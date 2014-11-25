#include "netsock.h"

int makesock(char* host, char* port,char server) {
	int sfd = 0;
	struct addrinfo hints;
	struct addrinfo *addr;
	memset(&hints,0,sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_ADDRCONFIG;
	if (server == 1) {
		hints.ai_flags = hints.ai_flags | AI_PASSIVE;
	}
	int gai = getaddrinfo(host,port,&hints,&addr);
	if (gai != 0) {
		printf("getaddrinfo error\n");
		printf(gai_strerror(gai));
	}
	/* borrow socket check from libsocket */
	struct addrinfo *check;
	for (check = addr; check != NULL; check = check->ai_next) {
		sfd = socket(check->ai_family,check->ai_socktype,check->ai_protocol);
		if (sfd < 0) continue;
		if (server == 1) {
			if (-1 != bind(sfd,check->ai_addr,check->ai_addrlen)) {
				printf("Listening\n");
				setupsock(sfd);
				break;
			}
		}
		else if (server == 0) {
			if (-1 != connect(sfd,check->ai_addr,check->ai_addrlen)) {
				printf("Connected");
				setupsock(sfd);
				break;
			}
		}
	}
	if (addr != NULL) freeaddrinfo(addr);
	return(sfd);
}

void setupsock(int sfd) {
	const int optVal = 1;
	const socklen_t optLen = sizeof(optVal);
	int rtn = setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, (void*) &optVal, optLen);
	if (rtn != 0) perror("setsockopt on sfd");
}
