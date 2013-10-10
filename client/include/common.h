#define BUFSIZE	80
#define PORT	8888
#define SERVER_IP	"10.10.83.62"

#define oops(str)	{ perror(str); exit(-1); }

struct info{
	char ip[INET_ADDRSTRLEN];
	in_port_t port;
	struct info *next;
};
extern struct info *head;

int gethostinfo(char **name, char **ip);//to get the host ip and name
int chart_server();//conected by others
int chart_client(struct info obj);//connect others
