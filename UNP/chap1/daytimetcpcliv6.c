#include <unp.h>
int main(int argc, char *argv[])
{
	int sockfd;
	char recline[MAXLINE + 1];
	if ( 2 > argc )
	  err_quit("usage: a.out <IPaddress>");
	if ( (sockfd = socket(AF_INET6, SOCK_STREAM, 0)) < 0)
	  err_sys("socket error");

	struct sockaddr_in6 servaddr;
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin6_family = AF_INET6;
	servaddr.sin6_port = htons(13);
	if ( inet_pton(AF_INET6, argv[1], &servaddr.sin6_addr) <= 0)
	  err_quit("inet_pton error for %s", argv[1]);

	if ( connect(sockfd, (SA*) &servaddr, sizeof(servaddr)) < 0)
	  err_sys("connect error");

	int n;
	while( (n = read(sockfd, recline, MAXLINE)) > 0)
	{
	  recline[n] = 0;
	  if ( fputs(recline, stdout) == EOF)
		err_sys("fputs error");
	}

	if (n < 0)
	  err_sys("read error");
	exit(0);
}
