/*
 * A simple Telnet clone designed for easy remote access
 * By Jacob Adams <tookmund@gmail.com>
 */
 
#include "netsock.h"
#define ADDR 12 	/* Length of an IPv4 address plus null */
#define SHELL "/bin/sh" /* Program to use as shell */
#define PORT "8888"	/* The port netshelld will listen on */

/* Function run by child proccesses to network the shell */
void child(int sfd) {
	dup2(sfd,0);
	dup2(sfd,1);
	dup2(sfd,2);
	char* args[] = {SHELL};
	execv(SHELL,args);
}

int main (void) {
	int srv = makesock(NULL,PORT,1);
	listen(srv,10);
	for (;;) {
		printf("Waiting...\n");
		int sfd = accept(srv,NULL,NULL);
		printf("\nForking\n");
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
			printf("\nConnected\n");
		}
	}
	return 0;
}

