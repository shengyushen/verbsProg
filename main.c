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

//common rdma
#include "def.h"
#include "main.h"
//socket exchange parameter
#include "ssysocket.h"
#include "rdma_common.h"
#include "print_rdma.h"
//checking device
#include "chk.h"
//only for rc test
#include "rctest.h"
//for srq rc test
//#include "srqtest.h"

bool isserver(int argc, char ** argv,char** ppServerHostNmae) {
	bool isServer;
	assert(argc>=4);
	if(strcmp(argv[3],"server")==0)  {
		isServer = true ;
	} else if (strcmp(argv[3],"client") == 0) {
		assert(argc>=5);
		isServer = false ;
		*ppServerHostNmae = argv[4];
	} else assert(0);
	return isServer;
}

bool issrq(int argc, char** argv) {
	return (strcmp(argv[1],"srqtest")==0);
}

bool isrc(int argc, char** argv) {
	return (strcmp(argv[1],"rctest")==0);
}

int main(int argc, char** argv) {
	if(argc<2) {
		printf("Usage : query_device.exe <operation type> ...\n");
		printf("        chk\n");
		printf("        rctest <ib dev name> server\n");
		printf("        rctest <ib dev name> client <server host name>\n");
		return 0;
	}
	//check status
	if(strcmp(argv[1],"chk") == 0)
		chk();
	else if(
		isrc(argc,argv) || 
		issrq(argc,argv) 
	) { //testing rdma
		struct arg_ctx arg_ctx;
		//find out the server and client
		arg_ctx.isServer = isserver(argc,argv,&(arg_ctx.pServerHostNmae));

		//init the socket to exchange the parameters
		int confd;
		int fd = init_socket(arg_ctx.isServer,arg_ctx.pServerHostNmae,&confd);
		arg_ctx.sockfd = arg_ctx.isServer?confd:fd;

		arg_ctx.pDevname = argv[2];

		arg_ctx.issrq=issrq(argc,argv);

		//real test
		rctest(&arg_ctx);

		if(arg_ctx.isServer) 
			close(confd);
		close(fd);

		if(arg_ctx.isServer)
			printf("server exit\n");
		else
			printf("client exit\n");
	} else 
		assert(0);
	
}


