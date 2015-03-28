/*
 * A simple Telnet clone designed for easy remote access
 * By Jacob Adams <tookmund@gmail.com>
 */
#include <sys/select.h>
#include <unistd.h>
#include "netsock.h"
#define MAXBUF 1 /* Maximim buffer size for reads and writes */

int main (int argc, char* argv[]) {
	if (argc < 2) printf("Usage: %s host port\n",argv[0]);
	else {
		int sfd = makesock(argv[1],argv[2],0);
		char buf [MAXBUF+1];
		memset(&buf,0,sizeof(buf));

		fd_set masterinp;
		FD_ZERO(&masterinp);
		FD_SET(0,&masterinp);
		FD_SET(sfd,&masterinp);
		
		fd_set masterout;
		FD_ZERO(&masterout);
		FD_SET(0,&masterout);
		FD_SET(sfd,&masterout);

		struct timeval timer;
		memset(&timer,0,sizeof(timer));
		int data = 0; // if there is data waiting to be written
		int all = 0; // amount of data
		while(1) {
			printf("Loop\n");
			fd_set inp = masterinp;
			fd_set out = masterout;
			int sel = select(sfd+1,&inp,&out,NULL,&timer);
			if (sel < 0) {
				perror("Select");
			}
			else {
				if(FD_ISSET(0,&inp)) {
					printf("input is available");
					if (!data) {
						all = read(0,buf,sizeof(buf));
						printf("Read (%s)",buf);
						data = 1;
					}
				}
				else if (FD_ISSET(sfd,&out)) {
					if(data) {
						int w = write(sfd,buf,sizeof(buf));
						if(w == all) {
							data = 0;
						}
					}
				}
			}
		}
	}
	return 0;
}
