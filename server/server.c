#include <stdio.h>
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
	int port = PORT;
	int fd;
	struct sockaddr_in ser_addr, cln_addr;
	char buffer[BUFSIZE];
	int addrlen = 0;
	int nbyte = 0;

	struct hostent *host = (struct hostent *)malloc(sizeof(struct hostent));
	host = gethostbyname("localhost");
	
	bzero(&ser_addr, sizeof(struct sockaddr));
	bzero(&cln_addr, sizeof(struct sockaddr));
	ser_addr.sin_family = AF_INET;
	ser_addr.sin_addr = *((struct in_addr *)host->h_addr);
	ser_addr.sin_port=htons(port);

	if((fd = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
		oops("create socket failed\n");

	if(bind(fd, (struct sockaddr *)&ser_addr, sizeof(struct sockaddr)) == -1)
		oops("bind the socket failed\n");

	printf("the server is ready now\n");
	while(1)
	{
		if((nbyte = recvfrom(fd, buffer, BUFSIZE, 0, (struct sockaddr *)&cln_addr, &addrlen)) < 0)
			oops("recevie failed\n");
		printf("this is from client: %s\n", buffer);


	printf("%d\n", nbyte);
	printf("%d\n", cln_addr.sin_family);
	printf("%s\n", inet_ntoa(cln_addr.sin_addr));
	printf("%d\n", cln_addr.sin_port);

		strcpy(buffer, "it's done");
		if(sendto(fd, buffer, strlen(buffer)+1, 0, (struct sockaddr *)&cln_addr, sizeof(struct sockaddr)) == -1)
			oops("send the result failed\n");
	}

	printf("the servers will shutdown\n");
	close(fd);

	return 0;
}
