#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <errno.h>
#include <stdlib.h>
#include <arpa/inet.h>


/**
 *filename:simple_udpserver.c
 *purpose:基本编程步骤说明， 演示UDP编程的服务器端编程步骤
 */
int main(int argc, char *argv[])
{
  struct sockaddr_in servaddr,cliaddr;
  int sockfd;
  socklen_t clilen;
  int len;
  char buff[128];
  
  /*创建socket，关键在于在SOCK_DGRAM*/
  if((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1){
    perror("socket");
    exit(errno);
  }
  else
    printf ("create socket.\n");

  memset(&servaddr, 0, sizeof(struct sockaddr_in));
  /*设置地址和信息*/
 servaddr.sin_family = AF_INET;
 if (argv[2])
   servaddr.sin_port = htons(atoi(argv[2]));
 else
   servaddr.sin_port = htons(7838);
 if (argv[1])
  servaddr.sin_addr.s_addr = inet_addr(argv[1]);
 else
   servaddr.sin_addr.s_addr = INADDR_ANY;

  /*绑定地址和端口信息*/
  if((bind(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr))) == -1){
    perror("bind");
    exit(errno);
  }else
    printf("bind address to socket.\n\r");
  
  /*循环接收数据*/
  clilen = sizeof(cliaddr);
  for(  ; ; ){
    len = recvfrom(sockfd, buff, sizeof(buff), 0, (struct sockaddr*)&cliaddr, &clilen);
    if(len < 0){
      perror("recvfrom");
      exit(errno);
    }
    buff[len] = '\0';
    printf ("收到来自%s：%d的消息：%s\n\r",
	    inet_ntoa(cliaddr.sin_addr), ntohs(cliaddr.sin_port), buff);
    
   }
  return 0;
}
