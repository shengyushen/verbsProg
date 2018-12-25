struct rctest_context {
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


void check_status();
void rctest(char * devname,int isServer, char* servername, int fd);

