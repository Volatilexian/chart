#include <stdio.h>
#include <netdb.h>
#include <string.h>
#include <strings.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "include/common.h"

struct info *head = NULL;

/*******************************************************
 *connect to the server, and get the user information
 *******************************************************/
int init()
{
	int fd;
	struct sockaddr_in	ser_addr;
	char buffer[BUFSIZE];
	int addrlen = 0;
	int port = PORT;
	char addr_p[INET_ADDRSTRLEN];
	struct info *info_p = NULL;

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

	do{//get the list of the online ip
		info_p = (struct info *)malloc(sizeof(struct info));
		if(recvfrom(fd, info_p, sizeof(struct info), 0, \
				(struct sockaddr *)&ser_addr, &addrlen) == -1)
			oops("recevie the statu failed\n");//get the information from server
		printf("the friend ip: %s, port: %d\n", info_p->ip, info_p->port);
		
		if(info_p->next == NULL)//it's the last one in the list
			break;
		info_p->next = head;
		head = info_p;
	}while(1);
	info_p->next = head;
	head = info_p;

	close(fd);

	return 0;
}

/*************************************************
 *print the information
 *************************************************/
void print()
{
}

/*************************************************
 *give the simple user interface
 *************************************************/
int main()
{
	init();//connect the server and get the information


}
