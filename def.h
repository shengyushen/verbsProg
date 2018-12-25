#define SSY_IBV_PORTNUM 1
#define SSY_TCP_PORTNUM 666
#define SSY_QKEY  0

struct ssy_context {
	struct ibv_device * pActiveDev;
	struct ibv_device ** ppdev;
	struct ibv_context * pctx;
	struct ibv_pd *ppd;
	//buffer property
	int bufsize ;
	void * pbuf ;
	int perm;
	struct ibv_mr * pmr;
	struct ibv_comp_channel * pCompCh;
	//cq property
	int cqe_num;
	struct ibv_cq * pcq;
	struct ibv_qp_init_attr qpinitattr;
	struct ibv_qp * pqp;
};


