#include <stdio.h>
#include <netdb.h>
#include <string.h>
#include <strings.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define oops(str)	{ perror(str); exit(-1); }
#define BUFSIZE	80
#define PORT	8888
#define SERVER_IP	"10.10.83.62"


/*******************************************************
 *connect to the server, and get the user information
 *******************************************************/
int main(int argc, char *argv[])
{
	int fd;
	struct sockaddr_in	ser_addr;
	char buffer[BUFSIZE];
	int addrlen = 0;
	int port = PORT;
	char addr_p[INET_ADDRSTRLEN];

	bzero(&ser_addr, sizeof(struct sockaddr_in));
	ser_addr.sin_family = AF_INET;
	inet_pton(AF_INET, SERVER_IP, &ser_addr.sin_addr.s_addr);
	ser_addr.sin_port = htons(port);

	if(-1 == (fd = socket(ser_addr.sin_family, SOCK_DGRAM, 0)))
		oops("create socket failed\n");

	printf("ready to connet...\n");

//	printf("input what u want to say: ");
//	fgets(buffer, BUFSIZE, stdin);
	strcpy(buffer, "hi, I'm coming.");
	addrlen = sizeof(struct sockaddr_in);
	if(sendto(fd, buffer, strlen(buffer)+1, 0, \
			(struct sockaddr *)&ser_addr, sizeof(struct sockaddr)) == -1)
		oops("send message failed\n");//connet the server

	if(recvfrom(fd, buffer, BUFSIZE, 0, \
			(struct sockaddr *)&ser_addr, &addrlen) == -1)
		oops("recevie the statu failed\n");//affirm the connect

	inet_ntop(AF_INET, &ser_addr.sin_addr.s_addr, addr_p, sizeof(addr_p));
	printf("connect successfully\n");
	printf("the server ip: %s, port: %d\n", addr_p, ntohs(ser_addr.sin_port));
	if(recvfrom(fd, buffer, BUFSIZE, 0, \
			(struct sockaddr *)&ser_addr, &addrlen) == -1)
		oops("recevie the statu failed\n");//get the information from server
	printf("the friend ip, port: %s\n", buffer);
	close(fd);

	return 0;
}
