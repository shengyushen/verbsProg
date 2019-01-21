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

#include "common.h"
#include "rdma_common.h"
#include "print_rdma.h"
void chk() {
	int num_dev;
	struct ibv_device ** ppdev = ibv_get_device_list(&num_dev);
	printf("num_dev %d\n",num_dev);
	
	//the list of dev
	for(int i=num_dev-1;i>=0;i--) {
		printf("============= Dev %s ===============\n",ppdev[i]->name);
		print_ibv_device(ppdev[i]);

		struct ibv_context * pctx = ibv_open_device(ppdev[i]);
		print_ibv_context(ppdev[i],pctx);
		ibv_close_device(pctx);

		printf("\n");
	}
	ibv_free_device_list(ppdev);
}


