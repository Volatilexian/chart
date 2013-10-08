#include <stdio.h>
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
	int port = PORT;

	bzero(&ser_addr, sizeof(struct sockaddr_in));
	ser_addr.sin_family = AF_INET;
	ser_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	ser_addr.sin_port = htons(port);

	if(-1 == (fd = socket(ser_addr.sin_family, SOCK_DGRAM, 0)))
		oops("create socket failed\n");

	printf("the client is ready now\n");

	printf("input what u want to say: ");
	scanf("%s", buffer);
	addrlen = sizeof(struct sockaddr_in);
	if(sendto(fd, buffer, strlen(buffer), 0, (struct sockaddr *)&ser_addr, sizeof(struct sockaddr)) == -1)
		oops("send message failed\n");

	if(recvfrom(fd, buffer, BUFSIZE, 0, (struct sockaddr *)&ser_addr, &addrlen) == -1)
		oops("recevie the statu failed\n");

	printf("the statu: %s\n", buffer);
	close(fd);

	return 0;
}
