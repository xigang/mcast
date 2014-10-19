#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <stdlib.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>

#define BUFLEN 255

/**
 *filename:mcastclient.c
 *purpose:演示组播编程的接本步骤，其实就是一个基本的UDP客户端(组播客户端)
 */
int main(int argc, char *argv[])
{
  struct sockaddr_in servaddr, cliaddr;
  int sockfd;
  char recmsg[BUFLEN + 1];
  unsigned int socklen;

  /*创建socket用于UDP通讯*/
  if((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1){
    printf ("socket creating error\n");
    exit(1);
  }
  socklen = sizeof(struct sockaddr_in);
  
  /*设置对方的端口号和IP信息*/
  memset(&servaddr, 0, socklen);
  servaddr.sin_family = AF_INET;
  if(argv[2])
    servaddr.sin_port = htons(atoi(argv[2]));
  else
    servaddr.sin_port = htons(7838);
  
  if(argv[1]){/*注意这里设置对方地址是指组播地址， 而不是对方的实际IP地址*/
    if(inet_pton(AF_INET, argv[1], &servaddr.sin_addr) <= 0){
      printf ("wrong group address!\n");
      exit(0);
    }
  }else{
    printf("no group address!\n");
    exit(0);
  }

  /*设置自己的端口和IP信息*/
  memset(&cliaddr, 0, socklen);
  cliaddr.sin_family = AF_INET;
  if(argv[4])
    cliaddr.sin_port = htons(atoi(argv[4]));
  else
    cliaddr.sin_port = htons(23456);

  if(argv[3]){
    if(inet_pton(AF_INET, argv[3], &cliaddr.sin_addr) <= 0){
      printf ("self ip address error!\n");
      exit(0);
    }
  }else
    cliaddr.sin_addr.s_addr = INADDR_ANY;

  /*绑定自己的端口和IP地址到socket上*/
  if(bind(sockfd, (struct sockaddr*)&cliaddr, sizeof(struct sockaddr_in)) == -1){
    printf("bind error!\n");
    exit(0);
  }
  
  /*循环接受用户输入的信息发送组播消息*/
  for( ; ; ){
    /*接受用户输入*/
    bzero(recmsg, BUFLEN + 1);
    if(fgets(recmsg, BUFLEN, stdin) == (char*)EOF)
      exit(0);
   /*发送消息*/
    if(sendto(sockfd, recmsg, strlen(recmsg), 0, (struct sockaddr*)&servaddr, 
	      sizeof(struct sockaddr_in)) < 0){
      printf("sendto error!\n");
      exit(3);
    }
    printf("'%s'send ok\n", recmsg);
  }
  return 0;
}
