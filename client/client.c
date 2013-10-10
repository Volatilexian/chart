#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <errno.h>
#include <netdb.h>
#include <string.h>
#include <strings.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define oops(str)	{ perror(str); exit(-1); }
#define BUFSIZE	80
#define PORT	8888

int main(int argc, char *argv[])
{
	int fd;
	struct sockaddr_in	ser_addr;
	char buffer[BUFSIZE];
	int addrlen = 0;
	in_port_t port = PORT;
	char addr_p[INET_ADDRSTRLEN];

	bzero(&ser_addr, sizeof(struct sockaddr_in));
	ser_addr.sin_family = AF_INET;
	inet_pton(AF_INET, "127.0.0.1", &ser_addr.sin_addr.s_addr);
	ser_addr.sin_port = htons(port);

	if(-1 == (fd = socket(ser_addr.sin_family, SOCK_DGRAM, 0)))
		oops("create socket failed\n");

	inet_ntop(AF_INET, &ser_addr.sin_addr.s_addr, addr_p, sizeof(addr_p));
	printf("ip: %s, port: %d\n", addr_p, ntohs(ser_addr.sin_port));
	printf("the client is ready now\n\n");

	printf("input what u want to say: ");
	fgets(buffer, BUFSIZE, stdin);
	addrlen = sizeof(struct sockaddr_in);
	if(sendto(fd, buffer, strlen(buffer)+1, 0, (struct sockaddr *)&ser_addr, addrlen) == -1)
		oops("send message failed\n");

	if(recvfrom(fd, buffer, BUFSIZE, 0, (struct sockaddr *)&ser_addr, &addrlen) == -1)
		oops("recevie the statu failed\n");

	printf("the statu: %s\n", buffer);
	inet_ntop(AF_INET, &ser_addr.sin_addr.s_addr, addr_p, sizeof(addr_p));
	printf("the server ip: %s, port: %d\n\n", addr_p, ntohs(ser_addr.sin_port));
	close(fd);

	return 0;
}
