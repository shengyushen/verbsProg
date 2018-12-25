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
#include "datastruct.h"
#include "print_rdma.h"
#include "query_device.h"
#include "ssysocket.h"

int main(int argc, char** argv) {
	if(argc<2) {
		printf("Usage : query_device.exe <operation type> ...\n");
		printf("        chk\n");
		printf("        rdma <ib dev name> server\n");
		printf("        rdma <ib dev name> client <server host name>\n");
		return 0;
	}
	//check status
	if(strcmp(argv[1],"chk") == 0)
		check_status();
	else if(strcmp(argv[1],"rdma") == 0) {
	//allocate context
		int isServer;
		char * pServerHostNmae = NULL;
		assert(argc>=4);
		if(strcmp(argv[3],"server")==0)  {
			isServer = 1 ;
		} else if (strcmp(argv[3],"client") == 0) {
			assert(argc>=5);
			isServer = 0 ;
			pServerHostNmae = argv[4];
		} else assert(0);
		int confd;
		int fd = init_socket(isServer,pServerHostNmae,&confd);
		rdma(argv[2],isServer,pServerHostNmae,isServer?confd:fd);
		if(isServer) 
			close(confd);

		close(fd);
		if(isServer)
			printf("server exit\n");
		else
			printf("client exit\n");
	} else 
		assert(0);
	
}


