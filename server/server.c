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
#include "include/common.h"

#define oops(str)	{ perror(str); exit(-1); }
#define PORT	8888
#define BUFSIZE 80

struct info{
	char ip[INET_ADDRSTRLEN];
	in_port_t port;
	struct info *next;
};
struct info *head = NULL;

/**************************************************
 *
 **************************************************/
int main(int argc, char *argv[])
{
	in_port_t port = PORT;
	int fd;
	struct sockaddr_in ser_addr, cln_addr;
	char buffer[BUFSIZE];
	socklen_t addrlen = 0;
	int nbyte = 0;
	char addr_p[INET_ADDRSTRLEN];
	char port_str[8];
	char *host_name;	//for the gethostinfo
	char *host_ip;		//for the gethostinfo
	struct info *info_p;

	bzero(&ser_addr, sizeof(struct sockaddr));
	bzero(&cln_addr, sizeof(struct sockaddr));
	ser_addr.sin_family = AF_INET;
	ser_addr.sin_addr.s_addr = INADDR_ANY;
	ser_addr.sin_port=htons(port);

	if((fd = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
		oops("create socket failed\n");

	if(bind(fd, (struct sockaddr *)&ser_addr, sizeof(struct sockaddr)) == -1)
		oops("bind the socket failed\n");

	gethostinfo(&host_name, &host_ip);
	printf("------------------------------------------\n");	//print the infomation of the server
	printf("the name of the server: %s\n", host_name);
	printf("ip: %s", host_ip);
	printf("port: %d\n", ntohs(ser_addr.sin_port));
	printf("------------------------------------------\n");
	printf("the server is ready now\n\n");

	while(1)
	{
		addrlen = sizeof(cln_addr);
		if((nbyte = recvfrom(fd, buffer, BUFSIZE, 0, (struct sockaddr *)&cln_addr, &addrlen)) < 0)
			oops("recevie failed\n");

		inet_ntop(AF_INET, &cln_addr.sin_addr.s_addr, addr_p, sizeof(addr_p));
		printf("client: ip: %s, port: %d\n", addr_p, ntohs(cln_addr.sin_port));
		printf("message: %s\n", buffer);

		strcpy(buffer, "yeah, I know.");
		if(sendto(fd, buffer, strlen(buffer)+1, 0, (struct sockaddr *)&cln_addr, addrlen) == -1)
			oops("send the result failed\n");
		
		
		//save the client ip and port
		info_p = (struct info *)malloc(sizeof(struct info));
		strcpy(info_p->ip, addr_p);
		info_p->port = ntohs(cln_addr.sin_port);
		info_p->next = head;
		head = info_p; 
		

		while(info_p != NULL)//it must be true when first run here
		{
			/*if(strcmp(info_p->ip, addr_p) != 0)
			{*/
				if(sendto(fd, info_p, sizeof(struct info), 0, (struct sockaddr *)&cln_addr, addrlen) == -1)
					oops("send the result failed\n");
			/*}
			else
			{
				strcpy(info_p->ip, "127.0.0.1");
				if(sendto(fd, info_p, sizeof(struct info), 0, (struct sockaddr *)&cln_addr, addrlen) == -1)
					oops("send the result failed\n");
			}*/
			info_p = info_p->next;
		}
	}

	printf("the servers will shutdown\n");
	close(fd);

	return 0;
}
