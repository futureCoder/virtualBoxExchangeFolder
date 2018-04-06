#include "unp.h"
#include <time.h>

int main(int argc, char *argv[])
{
  int listenfd;
  listenfd = Socket(AF_INET, SOCK_STREAM, 0);
  struct sockaddr_in servaddr;
  
  bzero(&servaddr, sizeof(servaddr));
  
  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  servaddr.sin_port = htons(13);

  Bind(listenfd, (SA*) & servaddr, sizeof(servaddr));
  
  Listen(listenfd, LISTENQ);
  for( ; ; )
  {
	int connfd = Accept(listenfd, (SA*)NULL, NULL);

	time_t ticks = time(NULL);
	char buff[MAXLINE];
	snprintf(buff, sizeof(buff), "%.24s\r\n", ctime(&ticks));
	Write(connfd, buff, strlen(buff));
	close(connfd);
  }
}
