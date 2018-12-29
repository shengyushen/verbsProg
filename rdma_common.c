#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <unistd.h>

#include <infiniband/verbs.h>
//#include <infiniband/verbs_exp.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
uint16_t getLid(struct ibv_context * pctx ) {
	struct ibv_device_attr dev_attr;
	ibv_query_device(pctx,&dev_attr);
	assert(dev_attr.phys_port_cnt == 1);
 	struct ibv_port_attr pa;
 	int resp = ibv_query_port(pctx,1,&pa);
	assert(resp==0);
	return pa.lid;
}

