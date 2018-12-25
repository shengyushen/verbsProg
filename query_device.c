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




void connect_rdma (uint32_t qpn_peer,uint32_t psn_peer,uint32_t psn,uint16_t lid_peer,struct ssy_context * pssyctx) {
	struct ibv_qp_attr attr;
	memset(&attr,0,sizeof(attr));

	//first set to init
	attr.qp_state = IBV_QPS_INIT;
	attr.pkey_index = 0;
	//this number can not be luan she, for example 6 leads to error
	attr.port_num = SSY_IBV_PORTNUM;//there is another port_num in ah_attr
	attr.qp_access_flags = 0;
	int resmod = ibv_modify_qp(pssyctx->pqp, &attr,IBV_QP_STATE | IBV_QP_PKEY_INDEX | IBV_QP_PORT | IBV_QP_ACCESS_FLAGS);
	if(resmod != 0) {
		printf("FATAL : resmod %d\n",resmod);
		printf("FATAL : EINVAL %d\n",EINVAL);
		printf("FATAL : ENOMEM %d\n",ENOMEM);
		printf("errno string : %s\n",strerror(errno));
		fflush(stdout);
		assert(0);
	}

	//then set to RTR
	memset(&attr,0,sizeof(attr));
	attr.qp_state = IBV_QPS_RTR;
	attr.path_mtu = IBV_MTU_4096;
	attr.dest_qp_num = qpn_peer;
	attr.rq_psn = psn_peer;
	attr.max_dest_rd_atomic= 1;
	attr.min_rnr_timer= 12; // this is recommandded
	attr.ah_attr.is_global = 0; // this is the GTH
	attr.ah_attr.dlid      = lid_peer ;
	attr.ah_attr.sl = 1; // this should be set in the MPI middle ware
	attr.ah_attr.src_path_bits = 0; // this is the source routing
	attr.ah_attr.port_num = SSY_IBV_PORTNUM;

	resmod = ibv_modify_qp(pssyctx->pqp,&attr,IBV_QP_STATE | IBV_QP_AV | IBV_QP_PATH_MTU | IBV_QP_DEST_QPN | IBV_QP_RQ_PSN | IBV_QP_MAX_DEST_RD_ATOMIC | IBV_QP_MIN_RNR_TIMER);
	if(resmod != 0) {
		printf("FATAL : resmod %d\n",resmod);
		printf("FATAL : EINVAL %d\n",EINVAL);
		printf("FATAL : ENOMEM %d\n",ENOMEM);
		printf("errno string : %s\n",strerror(errno));
		fflush(stdout);
		assert(0);
	}

	//finally to RTS
	memset(&attr,0,sizeof(attr));
	attr.qp_state = IBV_QPS_RTS;
	attr.timeout = 14;
	attr.retry_cnt =7;
	attr.rnr_retry = 7;
	attr.sq_psn = psn;
	attr.max_rd_atomic =1;

	resmod = ibv_modify_qp(pssyctx->pqp,&attr,IBV_QP_STATE | IBV_QP_TIMEOUT | IBV_QP_RETRY_CNT | IBV_QP_RNR_RETRY | IBV_QP_SQ_PSN | IBV_QP_MAX_QP_RD_ATOMIC); //these alst masks are used to tell the stack that the field to be modified
	assert(resmod == 0);
}

void getDevice_rdma(char * devname,struct ibv_device ** ppActiveDev,struct ibv_device *** pppdev) {
	int num_dev;
	struct ibv_device ** ppdev = ibv_get_device_list(&num_dev);
	//the list of dev
	for(int i=num_dev-1;i>=0;i--) {
		if(strcmp(ppdev[i]->name,devname) == 0) {
			*ppActiveDev = ppdev[i];
			break;
		}
	}
	assert(*ppActiveDev);
	*pppdev=ppdev;
}

void send_rdma(void * pbuf,int bufsize, struct ibv_mr * pmr,struct ibv_qp * pqp) {
	struct ibv_sge sge;
	memset(&sge,0,sizeof(sge));
	sge.addr = (uintptr_t)pbuf;
	sge.length = bufsize;
	sge.lkey = pmr->lkey;

	struct ibv_send_wr wr;
	memset(&wr,0,sizeof(wr));
	wr.next = NULL; //this is used in linked list in driver, I dont use it
	wr.sg_list = &sge;
	wr.num_sge = 1;
	wr.opcode = IBV_WR_SEND;
	wr.send_flags = IBV_SEND_SIGNALED; // this will generate a cqe

	struct ibv_send_wr * pbadwr;
	int ressend = ibv_post_send(pqp,&wr,&pbadwr);
	assert(ressend == 0);
}

void recv_rdma(struct ssy_context * pssyctx) {
	struct ibv_sge sge;
	memset(&sge,0,sizeof(sge));
	sge.addr = (uintptr_t)(pssyctx->pbuf);
	sge.length = pssyctx->bufsize;
	sge.lkey = (pssyctx->pmr)->lkey;

	struct ibv_recv_wr  wr;
	memset(&wr,0,sizeof(wr));
	wr.next = NULL; //this is used in linked list in driver, I dont use it
	wr.sg_list = &sge;
	wr.num_sge = 1;
	struct ibv_recv_wr * pbadwr;
	int resrecv = ibv_post_recv(pssyctx->pqp,&wr,&pbadwr);
	assert (resrecv ==0);
}

void wait4Comp_rdma(struct ssy_context * pssyctx) {
	struct ibv_wc wc;
	int num_comp;
	do {
		num_comp = ibv_poll_cq(pssyctx->pcq,1,&wc);
	} while(num_comp == 0);

	assert(num_comp >=0);
	assert(wc.status == IBV_WC_SUCCESS);
}




struct ssy_context * create_ssy_context(char * devname) {
	struct ssy_context * pssyctx = malloc(sizeof(struct ssy_context));

	getDevice_rdma(devname,&(pssyctx -> pActiveDev),&(pssyctx->ppdev));
	pssyctx->pctx = ibv_open_device(pssyctx->pActiveDev);

	//allocate pd
	pssyctx->ppd = ibv_alloc_pd(pssyctx->pctx);
	assert(pssyctx->ppd);
	assert(pssyctx->ppd->context == pssyctx->pctx);

	//memory alloc
	pssyctx->bufsize = 1024;
	pssyctx->pbuf = malloc(pssyctx -> bufsize);

	//memory registering
	pssyctx -> perm = IBV_ACCESS_LOCAL_WRITE | IBV_ACCESS_REMOTE_WRITE | IBV_ACCESS_REMOTE_READ | IBV_ACCESS_REMOTE_ATOMIC | IBV_ACCESS_MW_BIND | IBV_ACCESS_ZERO_BASED;
	pssyctx->pmr = ibv_reg_mr(pssyctx->ppd,pssyctx->pbuf,pssyctx -> bufsize,pssyctx->perm);

	//completion channel
	pssyctx-> pCompCh  = ibv_create_comp_channel( pssyctx->pctx );
	assert(pssyctx -> pCompCh);

	//completion queue
	pssyctx->cqe_num = 1024 ;
	pssyctx -> pcq = ibv_create_cq(pssyctx->pctx,pssyctx->cqe_num , NULL ,  // NULL means no private cq_context
																			pssyctx->pCompCh ,0  // 0 means empty vector
																			);
	assert(pssyctx -> pcq);

	// creating qp
	memset(&(pssyctx->qpinitattr),0,sizeof(struct ibv_qp_init_attr));
	(pssyctx->qpinitattr).send_cq = pssyctx->pcq;
	(pssyctx->qpinitattr).recv_cq =pssyctx-> pcq;
	(pssyctx->qpinitattr).cap.max_send_wr = 1;
	(pssyctx->qpinitattr).cap.max_recv_wr = 100;
	(pssyctx->qpinitattr).cap.max_send_sge = 1;
	(pssyctx->qpinitattr).cap.max_recv_sge = 1;
	(pssyctx->qpinitattr).qp_type = IBV_QPT_RC;

	pssyctx->pqp = ibv_create_qp(pssyctx->ppd,&(pssyctx->qpinitattr));
}

void destroy_ssy_context(struct ssy_context * pssyctx) {
	int resdesqp = ibv_destroy_qp(pssyctx->pqp);
	assert(resdesqp==0);

	int rescq = ibv_destroy_cq(pssyctx->pcq);
	assert(rescq == 0);

	int resch = ibv_destroy_comp_channel(pssyctx->pCompCh);
	assert(resch==0);
				
	int resMr = ibv_dereg_mr(pssyctx->pmr);
	assert(resMr==0);

	free(pssyctx->pbuf);

	int res1 = ibv_dealloc_pd(pssyctx->ppd);
	assert(res1==0);

	ibv_close_device(pssyctx->pctx);
	ibv_free_device_list(pssyctx->ppdev);
}

void rdma(char * devname,int isServer, char* servername, int fd) {

	struct ssy_context * pssyctx = create_ssy_context(devname);

	//get related lid and qpn for communicated between each other
	uint16_t lid= getLid(pssyctx->pctx);
	uint32_t qpn = (pssyctx->pqp)->qp_num;
	uint32_t psn = lrand48() & 0xffffff;
	printf("local lid : %d\n",lid);
	printf("local qpn : %d\n",qpn);
	printf("local psn : %d\n",psn);

	uint16_t lid_peer;
	uint32_t qpn_peer;
	uint32_t psn_peer;
	exchangeParm_socket(&lid,sizeof(lid),&lid_peer,sizeof(lid_peer),fd);
	exchangeParm_socket(&qpn,sizeof(qpn),&qpn_peer,sizeof(qpn_peer),fd);
	exchangeParm_socket(&psn,sizeof(psn),&psn_peer,sizeof(psn_peer),fd);

	printf("receive lid : %d\n",lid_peer);
	printf("receive qpn : %d\n",qpn_peer);
	printf("receive psn : %d\n",psn_peer);

	//connect
	connect_rdma(qpn_peer,psn_peer,psn,lid_peer,pssyctx);
	if(!isServer) { //client
		//receive
		//sleep(5);
		recv_rdma(pssyctx);
		wait4Comp_rdma(pssyctx);
		printf("RDMA receiving \n");
		for(int i=0;i<pssyctx->bufsize;i++) {
			//printf("pos %d is %d\n",i,((uint8_t *)pbuf)[i]);
			assert((i%256) == ((uint8_t *)(pssyctx->pbuf))[i]);
		}
	} else { //server
		//send
		for(int i=0;i<pssyctx->bufsize;i++) {
			((uint8_t *)(pssyctx->pbuf))[i]=i%256;
			//printf("server pos %d is %d\n",i,((uint8_t *)pbuf)[i]);
		}
		send_rdma(pssyctx->pbuf,pssyctx->bufsize,pssyctx->pmr,pssyctx->pqp);
		for(int i=0;i<pssyctx->bufsize;i++) {
			assert((i%256) == ((uint8_t *)(pssyctx->pbuf))[i]);
		}
		printf("server finish sending,waiting for completion\n");
		wait4Comp_rdma(pssyctx);
	}

	destroy_ssy_context(pssyctx);

}

void check_status() {
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

