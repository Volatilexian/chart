#include <stdio.h>
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
	int port = PORT;
	int fd;
	struct sockaddr_in ser_addr, cln_addr;
	char buffer[BUFSIZE];
	int nbyte = 0, addrlen = 0;
	
	bzero(&ser_addr, sizeof(struct sockaddr));
	bzero(&cln_addr, sizeof(struct sockaddr));
	ser_addr.sin_family = AF_INET;
	ser_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	ser_addr.sin_port=htons(port);

	if((fd = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
		oops("create socket failed\n");

	if(bind(fd, (struct sockaddr *)&ser_addr, sizeof(struct sockaddr)) == -1)
		oops("bind the socket failed\n");

	printf("the server is ready now\n");
	while(1)
	{
		if((nbyte = recvfrom(fd, buffer, BUFSIZE, 0, \
			(struct sockaddr *)&cln_addr, &addrlen)) < 0)
			oops("recevie failed\n");
		printf("this is from client: %s\n", buffer);

		if(sendto(fd, DONE, strlen(DONE), 0, (struct sockaddr *)&cln_addr, addrlen) == -1)
			oops("send the result failed\n");
	}

	printf("the servers will shutdown\n");
	close(fd);

	return 0;
}
