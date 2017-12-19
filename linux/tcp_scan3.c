/*
 * A TCP port scanner created by x373241884y(use thread)
 */

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <sys/select.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <pthread.h>

typedef struct port_segment
{
	void *addr;
	int  port;
} port_segment;

int maxThread = 100;
int threadUses = 0;


void *scan(void *arg)
{
	int   sock;         //socket descriptor
	pthread_detach(pthread_self());
	port_segment  *segment = (port_segment *)arg;
	fd_set fdset;
	struct timeval tv;
	struct sockaddr_in servaddr;   //socket structure

	sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP); //created the tcp socket
	if (sock == -1) {
		perror("Socket() error \n");
		threadUses--;
		exit(-1);
	}

	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(segment->port); //set the portno
	memcpy(&servaddr.sin_addr, segment->addr, sizeof(segment->addr));
	int revl = connect(sock, (struct sockaddr *)&servaddr, sizeof(servaddr));
	if (revl == 0) {
		printf("%-16s %-5d  Open             \n", inet_ntoa(*((struct in_addr*)segment->addr)), segment->port);
	}
	close(sock);         //socket descriptor
	threadUses--;
}
/* Main programs starts*/
int main(int argc, char **argv)
{

	int    port;         //port number
	int    start;         //start port
	int    end;         //end port
	int    socklen_t;         //socket length
	char    responce[1024];      //to receive data
	char   *message = "shell";     //data to send
	struct hostent *hostaddr;   //To be used for IPaddress

	pthread_t pth;
	if (argc < 4 ) {
		printf("TCP Port Scanner V0.3 By x373241884y\n\n");
		printf("Usage: ./tscan <IPaddress> <Start Port> <End Port>\n");
		return (EINVAL);
	}
	start = atoi(argv[2]);
	end   = atoi(argv[3]);
	hostaddr = gethostbyname( argv[1] ); //get the ip 1st argument
	int i = 0;
	for (port = start; port <= end; port++) {

		port_segment *portseg = (port_segment *)malloc(sizeof(port_segment));
		portseg->addr = hostaddr->h_addr;
		portseg->port = port;
		if (pthread_create(&pth, NULL, scan, (void *)portseg) != 0) {
			perror("pthread_creat fail!");
			exit(EXIT_FAILURE);
		}
		threadUses = threadUses + 1;
		while (threadUses >= maxThread) {
			sleep(5);
		}

	}

}