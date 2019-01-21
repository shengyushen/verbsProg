struct buffer_entry {
	int align_pagesize ;
	int bufsize ;
	void * pbuf ;
	int perm;
	struct ibv_mr * pmr;
};

struct rctest_context {
	struct ibv_device * pActiveDev;
	struct ibv_device ** ppdev;
	struct ibv_context * pctx;
	struct ibv_pd *ppd;

	//buffer property
	struct buffer_entry * psend;
	struct buffer_entry * precv;

	struct ibv_comp_channel * pCompCh;
	//cq property
	int sqe_num;
	int rqe_num;
	int cqe_num;
	struct ibv_cq * pcq;
	struct ibv_qp_init_attr qpinitattr;
	struct ibv_qp * pqp;
};

struct wc_wait_couter {
	long recv_cnt;
	long send_cnt;
};

bool is_wc_wait_empty (struct wc_wait_couter * pwccnt);

void check_status();
void rctest(struct arg_ctx_t *);

