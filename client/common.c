#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include "include/common.h"
#include <string.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>

int gethostinfo(char **name, char **ip)
{
	int fd;
	struct ifreq ifr;

	*name = (char *)malloc(10);
	gethostname(*name, 10);

	fd = socket(AF_INET, SOCK_DGRAM, 0);
	ifr.ifr_addr.sa_family = AF_INET;
	strncpy(ifr.ifr_name, "eth0", IFNAMSIZ-1);
	ioctl(fd, SIOCGIFADDR, &ifr);
	close(fd);
	*ip = (char *)malloc(20);
	sprintf(*ip, "%s\n", inet_ntoa(((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr));

	return 0;
}

