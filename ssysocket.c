#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <unistd.h>

#include <infiniband/verbs.h>
#include <infiniband/verbs_exp.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "def.h"

int init_socket(int isServer, char* servername,int * pconfd) {
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
		int ptonres = inet_pton(AF_INET , servername, & servaddr.sin_addr);
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

