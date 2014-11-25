/*
 * A simple Telnet clone designed for easy remote access
 * By Jacob Adams <tookmund@gmail.com>
 */

#include "netsock.h"
#define MAXBUF 1 /* Maximim buffer size for reads and writes */

int main (int argc, char* argv[]) {
	if (argc < 2) printf("Usage: %s host port\n",argv[0]);
	else {
		int sfd = makesock(argv[1],argv[2],0);
		char buf [MAXBUF+1];
		memset(&buf,0,sizeof(buf));
		int rb; /* number of bytes read */
		/* int wb; number of bytes written */
		for (;;) {
			memset(&buf,0,sizeof(buf));
			rb = 0;
			/* wb = 0; */
			rb = read(sfd, buf,MAXBUF);
			if (rb > 0) write(1,buf,MAXBUF);
			rb = read(0,buf,MAXBUF);
			if (rb > 0) write(1,buf,MAXBUF);
		}
	}
	return 0;
}
