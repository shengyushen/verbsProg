#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <infiniband/verbs.h>
#include <infiniband/verbs_exp.h>

void print_ibv_node_type(enum ibv_node_type node_type) {
	printf("node_type : ");
	switch(node_type) {
		case  IBV_NODE_UNKNOWN :
			printf("IBV_NODE_UNKNOWN\n");
			break;
		case IBV_NODE_CA :
			printf("IBV_NODE_CA\n");
			break;
		case IBV_NODE_SWITCH :
			printf("IBV_NODE_SWITCH\n");
			break;
		case IBV_NODE_ROUTER :
			printf("IBV_NODE_ROUTER\n");
			break;
		case IBV_NODE_RNIC :
			printf("IBV_NODE_RNIC\n");
			break;
		default :
			assert(0);
	}
}

void print_ibv_transport_type(enum ibv_transport_type tp) {
	printf("ibv_transport_type : ");
	switch(tp) {
		case IBV_TRANSPORT_UNKNOWN :
			printf("IBV_TRANSPORT_UNKNOWN\n");
			break;
		case IBV_TRANSPORT_IB :
			printf("IBV_TRANSPORT_IB\n");
			break;
		case IBV_TRANSPORT_IWARP :
			printf("IBV_TRANSPORT_IWARP\n");
			break;
		case IBV_EXP_TRANSPORT_SCIF :
			printf("IBV_EXP_TRANSPORT_SCIF\n");
			break;
		//case IBV_EXP_TRANSPORT_TYPE_START :
		//	printf("IBV_EXP_TRANSPORT_TYPE_START\n");
		//	break;
		default :
			assert(0);
	}
}

void print_ibv_device(struct ibv_device * pdev) {
	print_ibv_node_type(pdev->node_type);
	print_ibv_transport_type(pdev->transport_type);
	printf("Name of underlying kernel IB device : %s\n",pdev->name);
	printf("Name of uverbs device : %s\n",pdev->dev_name);
	printf("Path to infiniband_verbs class device in sysfs: %s\n",pdev->dev_path);
	printf("Path to infiniband class device in sysfs : %s\n",pdev->ibdev_path);
}

int main(int argc, char** argv) {
	int num_dev;
	struct ibv_device ** ppdev;

	ppdev = ibv_get_device_list(&num_dev);
	printf("num_dev %d\n",num_dev);
	for(int i=0;i<num_dev;i++) {
		printf("============= Dev %d ===============\n",i);
		print_ibv_device(ppdev[i]);
		printf("\n");
	}
	ibv_free_device_list(ppdev);
}

