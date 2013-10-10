#define oops(str)	{ perror(str); exit(-1); }

int gethostinfo(char **name, char **ip);
