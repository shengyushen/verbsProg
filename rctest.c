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
#include "main.h"
#include "rdma_common.h"
#include "print_rdma.h"
#include "rctest.h"
#include "ssysocket.h"

//FUNCTION
void 
getDevice_rdma
(
	char * devname,
	struct ibv_device ** ppActiveDev,
	struct ibv_device *** pppdev
) {
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


//FUNCTION
struct rctest_context * 
create_rctest_context(
	char * devname, 
	enum test_type_t test_type 
) {
	struct rctest_context * pssyctx = malloc(sizeof(struct rctest_context));

	getDevice_rdma(devname,&(pssyctx -> pActiveDev),&(pssyctx->ppdev));
	pssyctx->pctx = ibv_open_device(pssyctx->pActiveDev);

	//allocate pd
	pssyctx->ppd = ibv_alloc_pd(pssyctx->pctx);
	assert(pssyctx->ppd);
	assert(pssyctx->ppd->context == pssyctx->pctx);

	//for psend
	//memory alloc
	pssyctx->psend = malloc(sizeof(struct buffer_entry));
	pssyctx->psend->align_pagesize = sysconf(_SC_PAGESIZE);
	pssyctx->psend->bufsize = 1024;
	posix_memalign(&(pssyctx ->psend-> pbuf),pssyctx->psend->align_pagesize,pssyctx ->psend-> bufsize);
	//memory registering
	pssyctx->psend -> perm = IBV_ACCESS_LOCAL_WRITE | IBV_ACCESS_REMOTE_WRITE | IBV_ACCESS_REMOTE_READ | IBV_ACCESS_REMOTE_ATOMIC | IBV_ACCESS_MW_BIND | IBV_ACCESS_ZERO_BASED;
	pssyctx->psend ->pmr = ibv_reg_mr(pssyctx->ppd,pssyctx->psend->pbuf,pssyctx ->psend-> bufsize,pssyctx->psend->perm);

	//for precv
	//memory alloc
	pssyctx->precv = malloc(sizeof(struct buffer_entry));
	pssyctx->precv->align_pagesize = sysconf(_SC_PAGESIZE);
	pssyctx->precv->bufsize = 1024;
	posix_memalign(&(pssyctx ->precv-> pbuf),pssyctx->precv->align_pagesize,pssyctx ->precv-> bufsize);
	//memory registering
	pssyctx->precv -> perm = IBV_ACCESS_LOCAL_WRITE | IBV_ACCESS_REMOTE_WRITE | IBV_ACCESS_REMOTE_READ | IBV_ACCESS_REMOTE_ATOMIC | IBV_ACCESS_MW_BIND | IBV_ACCESS_ZERO_BASED;
	pssyctx->precv ->pmr = ibv_reg_mr(pssyctx->ppd,pssyctx->precv->pbuf,pssyctx ->precv-> bufsize,pssyctx->precv->perm);

	//completion channel
	pssyctx-> pCompCh  = ibv_create_comp_channel( pssyctx->pctx );
	assert(pssyctx -> pCompCh);

	//completion queue
	pssyctx->sqe_num = 1;
	pssyctx->rqe_num = 1024;
	//forcing the number of cqe to hold all sqe and rqe
	pssyctx->cqe_num = pssyctx->sqe_num + pssyctx->rqe_num ;
	pssyctx -> pcq = ibv_create_cq(pssyctx->pctx,pssyctx->cqe_num , NULL ,  // NULL means no private cq_context
																			pssyctx->pCompCh ,0  // 0 means empty vector
																			);
	assert(pssyctx -> pcq);

	if(issrq(test_type)) { //working on srq
		struct ibv_srq_init_attr attr;
		memset(&attr,0,sizeof(attr));
	//	attr.attr.max_wr = 
	}

	// creating qp
	memset(&(pssyctx->qpinitattr),0,sizeof(struct ibv_qp_init_attr));
	(pssyctx->qpinitattr).send_cq = pssyctx->pcq;
	(pssyctx->qpinitattr).recv_cq =pssyctx-> pcq;
	(pssyctx->qpinitattr).cap.max_send_wr = pssyctx->sqe_num;
	(pssyctx->qpinitattr).cap.max_recv_wr = pssyctx->rqe_num;
	(pssyctx->qpinitattr).cap.max_send_sge = 1;
	(pssyctx->qpinitattr).cap.max_recv_sge = 1;
	(pssyctx->qpinitattr).qp_type = IBV_QPT_RC;

	pssyctx->pqp = ibv_create_qp(pssyctx->ppd,&(pssyctx->qpinitattr));
}


//FUNCTION
void 
connect_rc 
(
	uint32_t qpn_peer,
	uint32_t psn_peer,
	uint32_t psn,
	uint16_t lid_peer,
	struct rctest_context * pssyctx
) {
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


//FUNCTION
void 
send_rc(
	void * pbuf,
	int bufsize, 
	struct ibv_mr * pmr,
	struct ibv_qp * pqp
) {
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
	if(ressend != 0) {
		printf("Error : ibv_post_send error %d\n",ressend);
		assert(0);
	}
}


//FUNCTION
void 
recv_rc(
	void * pbuf,
	int bufsize, 
	struct ibv_mr * pmr,
	struct ibv_qp * pqp
) {
	struct ibv_sge sge;
	memset(&sge,0,sizeof(sge));
	sge.addr = (uintptr_t)(pbuf);
	sge.length = bufsize;
	sge.lkey = pmr->lkey;

	struct ibv_recv_wr  wr;
	memset(&wr,0,sizeof(wr));
	wr.next = NULL; //this is used in linked list in driver, I dont use it
	wr.sg_list = &sge;
	wr.num_sge = 1;
	struct ibv_recv_wr * pbadwr;
	int resrecv = ibv_post_recv(pqp,&wr,&pbadwr);
	assert (resrecv ==0);
}

bool is_wc_wait_empty (struct wc_wait_couter * pwccnt) {
	return (pwccnt -> recv_cnt ==0) && (pwccnt -> send_cnt ==0);
}

//FUNCTION
void wait4Comp_rc(struct rctest_context * pssyctx, struct wc_wait_couter * pwccnt) {
	struct ibv_wc wc;
	int num_comp;
	printf("\n\n\n");
	while(!is_wc_wait_empty(pwccnt)) {
		num_comp = ibv_poll_cq(pssyctx->pcq,1,&wc);
		if(num_comp >0 ) {
			if(wc.status==IBV_WC_SUCCESS) {
				switch (wc.opcode) {
					case IBV_WC_RECV : 
						pwccnt->recv_cnt = pwccnt -> recv_cnt -1;
						printf("================recv wc================i\n");
						print_ibv_wc(&wc);
						break;
					case IBV_WC_SEND :
						pwccnt->send_cnt = pwccnt -> send_cnt -1;
						printf("================send wc=================\n");
						print_ibv_wc(&wc);
						break;
					default : 
						assert(0);
				}
			} else {
				print_ibv_wc(&wc);
				assert(0);
			}
		}
	}
	

	assert(num_comp >=0);
	assert(wc.status == IBV_WC_SUCCESS);
}

//FUNCTION
void 
destroy_rctest_context
(
	struct rctest_context * pssyctx
) {
	assert( ibv_destroy_qp(pssyctx->pqp) == 0 );
	assert( ibv_destroy_cq(pssyctx->pcq) == 0 );
	assert( ibv_destroy_comp_channel(pssyctx->pCompCh) == 0 );

	assert( ibv_dereg_mr(pssyctx->psend->pmr) ==0 ) ;
	free(pssyctx->psend->pbuf);
	free(pssyctx->psend);

	assert( ibv_dereg_mr(pssyctx->precv->pmr) ==0 ) ;
	free(pssyctx->precv->pbuf);
	free(pssyctx->precv);

	assert( ibv_dealloc_pd(pssyctx->ppd) == 0 );
	assert( ibv_close_device(pssyctx->pctx) == 0 );
	ibv_free_device_list(pssyctx->ppdev);
	free(pssyctx);
}




//FUNCTION
void 
rctest(
	struct arg_ctx_t * pargctx
) {

	struct rctest_context * pssyctx = create_rctest_context((pargctx->local_prop).pDevname,(pargctx->test_type));

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
	exchangeParm_socket(&lid,sizeof(lid),&lid_peer,sizeof(lid_peer),(pargctx->local_prop).sockfd);
	exchangeParm_socket(&qpn,sizeof(qpn),&qpn_peer,sizeof(qpn_peer),(pargctx->local_prop).sockfd);
	exchangeParm_socket(&psn,sizeof(psn),&psn_peer,sizeof(psn_peer),(pargctx->local_prop).sockfd);

	printf("receive lid : %d\n",lid_peer);
	printf("receive qpn : %d\n",qpn_peer);
	printf("receive psn : %d\n",psn_peer);

	//connect
	connect_rc(qpn_peer,psn_peer,psn,lid_peer,pssyctx);

	//init
	for(int i=0;i<pssyctx->psend->bufsize;i++) {
		((uint8_t *)(pssyctx->psend -> pbuf))[i]=i%256;
		//printf("server pos %d is %d\n",i,((uint8_t *)pbuf)[i]);
	}

	//send
	for(int i = 0 ; i<pargctx->numpack ; i++)
		send_rc(pssyctx->psend->pbuf,pssyctx->psend -> bufsize,pssyctx->psend->pmr,pssyctx->pqp);
	//receive
	for(int i = 0 ; i<pargctx->numpack ; i++)
		recv_rc(pssyctx->precv->pbuf,pssyctx->precv -> bufsize,pssyctx->precv->pmr,pssyctx->pqp);

	struct wc_wait_couter wccnt;
	wccnt.recv_cnt=pargctx->numpack;
	wccnt.send_cnt=pargctx->numpack;
	wait4Comp_rc(pssyctx,&wccnt);
	printf("finished waiting for completion\n");

	//checking
	for(int i=0;i<pssyctx->precv->bufsize;i++) {
		//printf("pos %d is %d\n",i,((uint8_t *)pbuf)[i]);
		if((i%256) != ((uint8_t *)(pssyctx->precv->pbuf))[i]) {
			printf("Error : not matching at i=%d value=%d\n",i,((uint8_t *)(pssyctx->precv->pbuf))[i]);
			assert(0);
		}
	}

	destroy_rctest_context(pssyctx);
}

