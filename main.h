struct arg_ctx {
	bool isServer;
	char * pServerHostNmae; //only make sense when isServer == 1

	char * pDevname;

	int  sockfd;

	bool issrq; // use din srq test
};
