#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <errno.h>

/**
 *filename:mcastserver.c
 *演示组播编程的基本步骤， 组播服务端， 关键在于加入组
 */
#define BUFLEN 255

int main(int argc, char *argv[])
{
  struct sockaddr_in servaddr;
  struct in_addr ia;
  int sockfd;
  char recmsg[BUFLEN + 1];
  unsigned int socklen, n;
  struct hostent *group;
  struct ip_mreq mreq;
  
  /*创建socket用于UDP通讯*/
  sockfd = socket(AF_INET, SOCK_DGRAM, 0);
  if(sockfd < 0){
    printf("socket creating err in udptalk\n");
    exit(1);
  }
  
  /*设置要加入组播的地址*/
  bzero(&mreq, sizeof(struct ip_mreq));
  if(argv[1]){
    if((group = gethostbyname(argv[1])) == (struct hostent*)0){
      perror("gethostbyname");
      exit(errno);
    }
  }else{
    printf ("you should give me a group address, 224.0.0.0-239.255.255.255\n");
    exit(errno);
  }
  
  bcopy((void*)group->h_addr, (void*)&ia, group->h_length);
  /*设置组地址*/
  bcopy(&ia, &mreq.imr_multiaddr.s_addr, sizeof(struct in_addr));
  
  /*设置发送组播消息的源主机的地址信息*/
  mreq.imr_interface.s_addr = htonl(INADDR_ANY);
  
  /*把本机加入组播地址， 即本机卡作为组播成员， 只要加入组才能收到组播消息*/
  if(setsockopt(sockfd, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq,
		sizeof(struct ip_mreq)) == -1){
    perror("setsockopt");
    exit(-1);
  }
  
  socklen = sizeof(struct sockaddr_in);
  memset(&servaddr, 0, socklen);
  servaddr.sin_family = AF_INET;
  if(argv[2])
    servaddr.sin_port = htons(atoi(argv[2]));
  else
    servaddr.sin_port = htons(7838);
  
  if(argv[1]){
    if(inet_pton(AF_INET, argv[1], &servaddr.sin_addr) < 0){
      printf("Wrong dest IP address!\n");
      exit(0);
    }
  }else{
    printf("no group address given, 224.0.0.0-239.255.255.255\n");
    exit(errno);
    }

  /*绑定自己的端口和IP信息到socket上*/
  if(bind(sockfd, (struct sockaddr*)&servaddr, sizeof(struct sockaddr_in)) == -1){
    printf("Bind error\n");
    exit(0);
  }

  /*循环j接收网络上来的组播消息*/
  for( ; ; ){
    bzero(recmsg, BUFLEN + 1);
    n = recvfrom(sockfd, recmsg, BUFLEN, 0,(struct sockaddr*)&servaddr, &socklen);
    if(n < 0){
      printf("recvfrom err in udptalk!\n");
      exit(4);
    }else{
      /*成功接收到数据报*/
      recmsg[n] = 0;
      printf ("peer:%s",recmsg);
    }
  }
  return 0;
}

