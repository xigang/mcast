#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <errno.h>
#include <stdlib.h>
#include <arpa/inet.h>

/**
 *filename:broadc-udpclient.c
 *purpose:基本编程步骤， 演示了UDP编程的广播客户端编程步骤
 */

int main(int argc, char *argv[])
{
  struct sockaddr_in servaddr;
  int sockfd;
  int addr_len;
  int len;
  char buff[128];
  int yes;

  /*创建socket，关键在于在SOCK_DGRAM*/
  if((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1){
    perror("socket");
    exit(errno);
  }else
    printf("created socket.\n\r");
  
  /*设置通讯方式为广播，即本程序发送的一个消息， 网络上所有主机均可以收到*/
  yes = 1;
  setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, &yes, sizeof(yes));
  /*唯一变化就是这一点*/
  
  /*设置对方的地址和端口信息*/
  servaddr.sin_family = AF_INET;
  if(argv[2])
    servaddr.sin_port = htons(atoi(argv[2]));
  else
    servaddr.sin_port = htons(7838);
  
  if(argv[1])
    servaddr.sin_addr.s_addr = inet_addr(argv[1]);
  else{
    printf("消息必须有一个接受者！\n");
    exit(0);
  }

  /*发送UDP消息*/
  addr_len = sizeof(servaddr);
  strcpy(buff, "hello , i'm here");
  len = sendto(sockfd, buff, strlen(buff), 0, (struct sockaddr*)&servaddr, addr_len);
  if(len < 0){
    printf ("\n\rsend error.\n\r");
    return 3;
  }

  printf ("send success.\n");
  
  return 0;
}

