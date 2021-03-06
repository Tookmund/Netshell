/*
 * A simple Telnet clone designed for easy remote access
 * By Jacob Adams <tookmund@gmail.com>
 */
#include <sys/select.h>
#include <unistd.h>
#include <fcntl.h>
#include "netsock.h"
#define MAXBUF 1025 /* Maximim buffer size for reads and writes */

int main (int argc, char* argv[]) {
	if (argc < 2) printf("Usage: %s host port\n",argv[0]);
	else {
		int sfd = makesock(argv[1],argv[2],0);

		fcntl(0,F_SETFL,O_NONBLOCK);
		fcntl(1,F_SETFL,O_NONBLOCK);
		fcntl(sfd,F_SETFL,O_NONBLOCK);

		char stdinbuf[MAXBUF];
		char sockbuf[MAXBUF];
		memset(stdinbuf,0,sizeof(stdinbuf));
		memset(sockbuf,0,sizeof(sockbuf));

		fd_set masterread;
		FD_ZERO(&masterread);
		FD_SET(0,&masterread);
		FD_SET(sfd,&masterread);

		fd_set masterwrite;
		FD_ZERO(&masterwrite);
		FD_SET(1,&masterwrite);
		FD_SET(sfd,&masterwrite);

		struct timeval timer;
		memset(&timer,0,sizeof(timer));

		char sockdata = 0; // if there is data waiting to be written from the socket
		int sockall = 0; // amount of data read from the socket
		int socksome = 0; //amount of data written

		char stdindata = 0; // if there is data waiting to be written from stdin
		int stdinall = 0; // amount of data read from stdin
		int stdinsome = 0;

		fd_set readfrom;
		fd_set writeto;
		while(1) {
			fprintf(stderr,"stdinbuf: (%s)\nsockbuf: (%s)\n",stdinbuf,sockbuf);
			readfrom = masterread;
			writeto  = masterwrite;
			int sel = select(sfd+1,&readfrom,&writeto,NULL,&timer);
			if (sel < 0) {
				perror("Select");
			}
			else {
				if (FD_ISSET(0,&readfrom)) {
					if (!stdindata) {
						stdinall = read(0,stdinbuf,sizeof(stdinbuf));
						stdindata = 1;
					}
				}
				if (FD_ISSET(sfd,&readfrom)) {
					if (!sockdata) {
						sockall = read(sfd,sockbuf,sizeof(sockbuf));
					}
				}
				if (FD_ISSET(sfd,&writeto)) {
					if(stdindata) {
						socksome += write(sfd,stdinbuf,sizeof(stdinbuf));
						if (socksome >= sockall) {
							stdindata = 1;
							sockall = 0;
						}
					}
				}
				if (FD_ISSET(1,&writeto)) {
					if(sockdata) {
						stdinsome += write(1,sockbuf,sizeof(sockbuf));
						if (stdinsome >= stdinall) {
							sockdata = 1;
							stdinall = 0;
						}
					}
				}
			}
		}
	}
	return 0;
}
