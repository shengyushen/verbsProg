int init_socket(int isServer, char* servername,int * pconfd);
void wait4data_socket(int fd, void* pdata,int len);
void exchangeParm_socket(void * pbufSend,int bufsizeSend,void * pbufRecv,int bufsizeRecv,int fd);


