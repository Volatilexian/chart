#include <stdio.h>
#include <unistd.h>
#include <ctype.h>
#include <errno.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <strings.h>

#define oops(str)	{ perror(str); exit(-1); }
#define PORT	8888
#define BUFSIZE 80
#define DONE	"it's done now!"

int main(int argc, char *argv[])
{
	in_port_t port = PORT;
	int fd;
	struct sockaddr_in ser_addr, cln_addr;
	char buffer[BUFSIZE];
	socklen_t addrlen = 0;
	int nbyte = 0;
	char addr_p[INET_ADDRSTRLEN];

	bzero(&ser_addr, sizeof(struct sockaddr));
	bzero(&cln_addr, sizeof(struct sockaddr));
	ser_addr.sin_family = AF_INET;
	ser_addr.sin_addr.s_addr = INADDR_ANY;
	ser_addr.sin_port=htons(port);

	if((fd = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
		oops("create socket failed\n");

	if(bind(fd, (struct sockaddr *)&ser_addr, sizeof(struct sockaddr)) == -1)
		oops("bind the socket failed\n");

	inet_ntop(AF_INET, &ser_addr.sin_addr.s_addr, addr_p, sizeof(addr_p));
	printf("ip: %s, port: %d\n", addr_p, ntohs(ser_addr.sin_port));
	printf("the server is ready now\n\n");

	while(1)
	{
		addrlen = sizeof(cln_addr);
		if((nbyte = recvfrom(fd, buffer, BUFSIZE, 0, (struct sockaddr *)&cln_addr, &addrlen)) < 0)
			oops("recevie failed\n");

		inet_ntop(AF_INET, &cln_addr.sin_addr.s_addr, addr_p, sizeof(addr_p));
		printf("client: ip: %s, port: %d\n", addr_p, cln_addr.sin_port);
		printf("message: %s\n", buffer);

		strcpy(buffer, DONE);
		if(sendto(fd, buffer, strlen(buffer)+1, 0, (struct sockaddr *)&cln_addr, addrlen) == -1)
			oops("send the result failed\n");
	}

	printf("the servers will shutdown\n");
	close(fd);

	return 0;
}
