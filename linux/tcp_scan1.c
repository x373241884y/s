/*
 * A TCP port scanner created by x373241884y(block)
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

/* Main programs starts*/
int main(int argc, char **argv)
{
	int   sock;         //socket descriptor
	int    port;         //port number
	int   start;         //start port
	int    end;         //end port
	char    responce[1024];      //to receive data
	char   *message = "shell";     //data to send
	struct hostent *hostaddr;   //To be used for IPaddress
	struct sockaddr_in servaddr;   //socket structure

	if (argc < 4 ) {
		printf("TCP Port Scanner V0.1 By x373241884y\n\n");
		printf("Usage: ./tscan <IPaddress> <Start Port> <End Port>\n");
		return (EINVAL);
	}
	start = atoi(argv[2]);
	end   = atoi(argv[3]);
	hostaddr = gethostbyname( argv[1] ); //get the ip 1st argument
	for (port = start; port <= end; port++) {

		// printf("detecting port %d\n", port);
		sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP); //created the tcp socket
		if (sock == -1) {
			perror("Socket()\n");
			return (errno);
		}


		servaddr.sin_family = AF_INET;
		servaddr.sin_port = htons(port); //set the portno
		memcpy(&servaddr.sin_addr, hostaddr->h_addr, hostaddr->h_length);
		int revl = connect(sock, (struct sockaddr *)&servaddr, sizeof(servaddr));
		if (revl == 0) {
			printf("%-16s %-5d  Open             \n", hostaddr->h_name, port);
		}
		close(sock);         //socket descriptor
	}

}