#include <unp.h>
int main(int argc, char *argv[])
{
	int sockfd;
	char recline[MAXLINE + 1];
	if ( 2 > argc )
	  err_quit("usage: a.out <IPaddress>");
	if ( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	  err_sys("socket error");

	struct sockaddr_in servaddr;
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(13);
	if ( inet_pton(AF_INET, argv[1], &servaddr.sin_addr) <= 0)
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
