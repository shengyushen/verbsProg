#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>

#include <infiniband/verbs.h>
#include <infiniband/verbs_exp.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#include "common.h"

void hostname2ip(char* phostname,char * pres) {
	struct addrinfo hints, *infoptr; // So no need to use memset global variables
	memset(&hints,0,sizeof(hints));
	memset(&infoptr,0,sizeof(infoptr));
  hints.ai_family = AF_INET; // AF_INET means IPv4 only addresses

  int result = getaddrinfo(phostname, NULL, &hints, &infoptr);
  if (result) {
      fprintf(stderr, "getaddrinfo for %s : %s\n",phostname, gai_strerror(result));
      exit(1);
  }

  struct addrinfo *p;
  char host[256];

  for (p = infoptr; p != NULL; p = p->ai_next) {
      getnameinfo(p->ai_addr, p->ai_addrlen, host, sizeof (host), NULL, 0, NI_NUMERICHOST);
			strcpy(pres,host);
			//printf("%s\n",pres);
			break;
  }

  freeaddrinfo(infoptr);
}

int init_socket(bool isServer, char* servername,int * pconfd) {
	int listenfd=socket(AF_INET , SOCK_STREAM ,0);
	assert(listenfd != -1);

	struct sockaddr_in servaddr;
	memset(&servaddr , 0 , sizeof(servaddr));
	servaddr.sin_family = AF_INET ;
	servaddr.sin_port = htons(SSY_TCP_PORTNUM);

	if(isServer) {
		servaddr.sin_addr.s_addr = htonl(INADDR_ANY) ;
		
		int bindres=bind(listenfd , (struct sockaddr *)&servaddr,sizeof(servaddr));
		assert(bindres != -1);

		int listenres = listen(listenfd,100);
		assert(listenres != -1);

		int confd = accept(listenfd,(struct sockaddr *)NULL,NULL);
		assert(confd != -1);
		*pconfd=confd;
	} else {
		//client
		char pipaddr[100];
		hostname2ip(servername,pipaddr);
		int ptonres = inet_pton(AF_INET , pipaddr, & servaddr.sin_addr);
		//int ptonres = inet_pton(AF_INET , servername, & servaddr.sin_addr);
		assert(ptonres >0);
		int connres = connect(listenfd,(struct sockaddr*)&servaddr,sizeof(servaddr));
		assert(connres>=0);
	}
	return listenfd;
}


void wait4data_socket(int fd, void* pdata,int len) {
	int sz = recv(fd,pdata,len,0);
	assert(sz=len);
	return ;
}

void exchangeParm_socket(void * pbufSend,int bufsizeSend,void * pbufRecv,int bufsizeRecv,int fd) {
	assert(send(fd,pbufSend,bufsizeSend,0));
	wait4data_socket(fd,pbufRecv,bufsizeRecv);
}

