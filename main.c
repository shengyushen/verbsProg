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
#include "common.h"
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

bool issrqtest(int argc, char** argv) {
	return (strcmp(argv[1],"srqtest")==0);
}

bool isrctest(int argc, char** argv) {
	return (strcmp(argv[1],"rctest")==0);
}

void fill_test_type(int argc, char ** argv , enum test_type_t * ptt) {
	if(isrctest(argc,argv)) 
		*ptt = TEST_TYPE_RCTEST;
	else if(issrqtest(argc,argv))
		*ptt = TEST_TYPE_SRQTEST;
	else
		assert(0);
}

void get_test_type_str(enum test_type_t tt, char * ss) {
	switch(tt) {
		case TEST_TYPE_RCTEST : sprintf(ss,"rctest"); break;
		case TEST_TYPE_SRQTEST : sprintf(ss,"srqtest");break;
		default : assert(0);
	}
}

int main(int argc, char** argv) {
	if(argc<2) {
		printf("Usage : query_device.exe <operation type> ...\n");
		printf("        chk\n");
		printf("        server  <ib dev name>\n");
		printf("        rctest  <ib dev name> <server host name>\n");
		printf("        srqtest <ib dev name> <server host name>\n");
		return 0;
	}
	//check status
	struct arg_ctx_t arg_ctx;
	if(strcmp(argv[1],"chk") == 0)
		chk();
	else if(strcmp(argv[1],"server") == 0) {
		// server side

		//generate all local_prop
		arg_ctx.local_prop.isServer = true;

		int confd;
		int fd = init_socket(true,NULL,&confd);
		arg_ctx.local_prop.sockfd = confd;

		arg_ctx.local_prop.pDevname = argv[2];


		//test type : get from client
		wait4data_socket(arg_ctx.local_prop.sockfd,
				&(arg_ctx.test_type),
				sizeof(arg_ctx.test_type)
				);
		chk_test_type_t(arg_ctx.test_type);
		char  pss [100];
		get_test_type_str(arg_ctx.test_type,pss);
		printf("Server : client test type %s\n",pss);

		rctest(&arg_ctx);
		
		close(confd);
		close(fd);
		printf("server exit\n");
	} else if( isrctest(argc,argv) || issrqtest(argc,argv) ) { //testing rdma
		// client side

		//find out the server and client
		arg_ctx.local_prop.isServer = false;
		//client side know the server name
		arg_ctx.pServerHostName = argv[3];

		//init the socket to exchange the parameters
		int confd;
		int fd = init_socket(arg_ctx.local_prop.isServer,
								arg_ctx.pServerHostName,
								&confd);
		arg_ctx.local_prop.sockfd = fd;

		arg_ctx.local_prop.pDevname = argv[2];

		//test type
		fill_test_type(argc,argv,&(arg_ctx.test_type));
		//exchange the test type
		assert(send(fd,&(arg_ctx.test_type),sizeof(arg_ctx.test_type),0));

		//real test
		rctest(&arg_ctx);

		close(fd);

		printf("client exit\n");
	} else 
		assert(0);
	
}


