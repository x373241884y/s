/*
 * A TCP port scanner created by x373241884y(use select)
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


/* Main programs starts*/
int main(int argc, char **argv)
{
	int   sock;         //socket descriptor
	int    port;         //port number
	int   start;         //start port
	int    end;         //end port
	int    socklen_t;         //socket length
	char    responce[1024];      //to receive data
	char   *message = "shell";     //data to send
	struct hostent *hostaddr;   //To be used for IPaddress
	struct sockaddr_in servaddr;   //socket structure
	fd_set fdset;
	struct timeval tv;

	if (argc < 4 ) {
		printf("TCP Port Scanner V0.2 By x373241884y\n\n");
		printf("Usage: ./tscan <IPaddress> <Start Port> <End Port>\n");
		return (EINVAL);
	}
	start = atoi(argv[2]);
	end   = atoi(argv[3]);
	hostaddr = gethostbyname( argv[1] ); //get the ip 1st argument
	for (port = start; port <= end; port++) {

		servaddr.sin_family = AF_INET;
		servaddr.sin_port = htons(port); //set the portno
		memcpy(&servaddr.sin_addr, hostaddr->h_addr, hostaddr->h_length);

		sock = socket(AF_INET, SOCK_STREAM, 0);  //created the ip socket
		if (sock == -1) {
			perror("Socket()\n");
			return (errno);
		}
		fcntl(sock, F_SETFL, O_NONBLOCK);
		connect(sock, (struct sockaddr *)&servaddr, sizeof(servaddr));

		FD_ZERO(&fdset);
		FD_SET(sock, &fdset);
		tv.tv_sec = 0;             /* 10 second timeout */
		tv.tv_usec = 500;

		if (select(sock + 1, NULL, &fdset, NULL, &tv) == 1)
		{
			int sock_error;
			int len = sizeof(sock_error);
			getsockopt(sock, SOL_SOCKET, SO_ERROR, &sock_error, &len);

			if (sock_error == 0) {
				close(sock);
				printf("%-16s %-5d  Open             \n", hostaddr->h_name, port);
			}
		}
		close(sock);         //socket descriptor
	}

}