#define SSY_IBV_PORTNUM 1
#define SSY_TCP_PORTNUM 666
#define SSY_QKEY  0

// local property
struct arg_ctx_local_t {
	bool isServer;
	char * pDevname;
	int  sockfd;
};

enum test_type_t {
	TEST_TYPE_MIN,
	TEST_TYPE_RCTEST,
	TEST_TYPE_IMMTEST,
	TEST_TYPE_SRQTEST,
	TEST_TYPE_MAX
};




struct arg_ctx_t {
	struct arg_ctx_local_t local_prop;
	enum test_type_t test_type;
	char * pServerHostName; //only make sense when isServer == 1
};


bool issrq(enum test_type_t test_type);
bool chk_test_type_t(enum test_type_t tsttype);
