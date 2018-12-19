#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <infiniband/verbs.h>
#include <infiniband/verbs_exp.h>

void print_ibv_node_type(enum ibv_node_type node_type) {
	printf("node_type : %s\n",ibv_node_type_str(node_type));
	//switch(node_type) {
	//	case  IBV_NODE_UNKNOWN :
	//		printf("IBV_NODE_UNKNOWN\n");
	//		break;
	//	case IBV_NODE_CA :
	//		printf("IBV_NODE_CA\n");
	//		break;
	//	case IBV_NODE_SWITCH :
	//		printf("IBV_NODE_SWITCH\n");
	//		break;
	//	case IBV_NODE_ROUTER :
	//		printf("IBV_NODE_ROUTER\n");
	//		break;
	//	case IBV_NODE_RNIC :
	//		printf("IBV_NODE_RNIC\n");
	//		break;
	//	default :
	//		assert(0);
	//}
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

void print_ibv_atomic_cap(enum ibv_atomic_cap acap) {
	switch(acap) {
		case IBV_ATOMIC_NONE :
			printf("\t IBV_ATOMIC_NONE\n");
			break;
		case IBV_ATOMIC_HCA :
			printf("\t IBV_ATOMIC_HCA\n");
			break;
		case IBV_ATOMIC_GLOB :
			printf("\t IBV_ATOMIC_GLOB\n");
			break;
		default:
			assert(0);
	}
}

void print_ibv_device_cap_flags(enum ibv_device_cap_flags dcf) {
	printf("\t device_cap_flags 0x%x\n",dcf);

	printf("\t IBV_DEVICE_RESIZE_MAX_WR == ");
	if((IBV_DEVICE_RESIZE_MAX_WR & dcf) !=0) printf("1\n");
	else printf("0\n");

	printf("\t IBV_DEVICE_BAD_PKEY_CNTR == ");
	if((IBV_DEVICE_BAD_PKEY_CNTR & dcf) !=0) printf("1\n");
	else printf("0\n");

	printf("\t IBV_DEVICE_BAD_QKEY_CNTR == ");
	if((IBV_DEVICE_BAD_QKEY_CNTR & dcf) !=0) printf("1\n");
	else printf("0\n");

	printf("\t IBV_DEVICE_RAW_MULTI == ");
	if((IBV_DEVICE_RAW_MULTI & dcf) !=0) printf("1\n");
	else printf("0\n");

	printf("\t IBV_DEVICE_AUTO_PATH_MIG == ");
	if((IBV_DEVICE_AUTO_PATH_MIG & dcf) !=0) printf("1\n");
	else printf("0\n");

	printf("\t IBV_DEVICE_CHANGE_PHY_PORT == ");
	if((IBV_DEVICE_CHANGE_PHY_PORT & dcf) !=0) printf("1\n");
	else printf("0\n");

	printf("\t IBV_DEVICE_UD_AV_PORT_ENFORCE == ");
	if((IBV_DEVICE_UD_AV_PORT_ENFORCE & dcf) !=0) printf("1\n");
	else printf("0\n");

	printf("\t IBV_DEVICE_CURR_QP_STATE_MOD == ");
	if((IBV_DEVICE_CURR_QP_STATE_MOD & dcf) !=0) printf("1\n");
	else printf("0\n");

	printf("\t IBV_DEVICE_SHUTDOWN_PORT == ");
	if((IBV_DEVICE_SHUTDOWN_PORT & dcf) !=0) printf("1\n");
	else printf("0\n");

	printf("\t IBV_DEVICE_INIT_TYPE == ");
	if((IBV_DEVICE_INIT_TYPE & dcf) !=0) printf("1\n");
	else printf("0\n");

	printf("\t IBV_DEVICE_PORT_ACTIVE_EVENT == ");
	if((IBV_DEVICE_PORT_ACTIVE_EVENT & dcf) !=0) printf("1\n");
	else printf("0\n");

	printf("\t IBV_DEVICE_SYS_IMAGE_GUID == ");
	if((IBV_DEVICE_SYS_IMAGE_GUID & dcf) !=0) printf("1\n");
	else printf("0\n");

	printf("\t IBV_DEVICE_RC_RNR_NAK_GEN == ");
	if((IBV_DEVICE_RC_RNR_NAK_GEN & dcf) !=0) printf("1\n");
	else printf("0\n");

	printf("\t IBV_DEVICE_SRQ_RESIZE == ");
	if((IBV_DEVICE_SRQ_RESIZE & dcf) !=0) printf("1\n");
	else printf("0\n");

	printf("\t IBV_DEVICE_N_NOTIFY_CQ == ");
	if((IBV_DEVICE_N_NOTIFY_CQ & dcf) !=0) printf("1\n");
	else printf("0\n");

	printf("\t IBV_DEVICE_MEM_WINDOW == ");
	if((IBV_DEVICE_N_NOTIFY_CQ & dcf) !=0) printf("1\n");
	else printf("0\n");

	printf("\t IBV_DEVICE_XRC == ");
	if((IBV_DEVICE_XRC & dcf) !=0) printf("1\n");
	else printf("0\n");

	printf("\t IBV_DEVICE_MEM_MGT_EXTENSIONS == ");
	if((IBV_DEVICE_MEM_MGT_EXTENSIONS & dcf) !=0) printf("1\n");
	else printf("0\n");

	printf("\t IBV_DEVICE_MEM_WINDOW_TYPE_2A == ");
	if((IBV_DEVICE_MEM_WINDOW_TYPE_2A & dcf) !=0) printf("1\n");
	else printf("0\n");

	printf("\t IBV_DEVICE_MEM_WINDOW_TYPE_2B == ");
	if((IBV_DEVICE_MEM_WINDOW_TYPE_2B & dcf) !=0) printf("1\n");
	else printf("0\n");

	printf("\t IBV_DEVICE_MANAGED_FLOW_STEERING == ");
	if((IBV_DEVICE_MANAGED_FLOW_STEERING & dcf) !=0) printf("1\n");
	else printf("0\n");


}

void print_ibv_device_attribute(struct ibv_device_attr * pda) {
	printf("\t firmware version : %s\n",pda->fw_ver);
	printf("\t node guid : 0x%lx\n",pda->node_guid);
	printf("\t sys image guid : 0x%lx\n",pda->sys_image_guid);
	printf("\t max_mr_size : 0x%lx\n",pda->max_mr_size);
	printf("\t page_size_cap : 0x%lx\n",pda->page_size_cap);
	printf("\t vendor_id : %ud\n",pda->vendor_id);
	printf("\t vendor_part_id : %ud\n",pda->vendor_part_id);
	printf("\t hw_ver : %ud\n",pda->hw_ver);
	printf("\t max_qp : 0x%x\n",pda->max_qp);
	printf("\t max_qp_wr : 0x%x\n",pda->max_qp_wr);
	print_ibv_device_cap_flags(pda->device_cap_flags);
	printf("\t max_sge : 0x%x\n",pda->max_sge);
	printf("\t max_sge_rd : 0x%x\n",pda->max_sge_rd);
	printf("\t max_cq : 0x%x\n",pda->max_cq);
	printf("\t max_cqe : 0x%x\n",pda->max_cqe);
	printf("\t max_mr : 0x%x\n",pda->max_mr);
	printf("\t max_pd : 0x%x\n",pda->max_pd);
	printf("\t max_qp_rd_atom : 0x%x\n",pda->max_qp_rd_atom);
	printf("\t max_ee_rd_atom : 0x%x\n",pda->max_ee_rd_atom);
	printf("\t max_res_rd_atom : 0x%x\n",pda->max_res_rd_atom);
	printf("\t max_qp_init_rd_atom : 0x%x\n",pda->max_qp_init_rd_atom);
	printf("\t max_ee_init_rd_atom : 0x%x\n",pda->max_ee_init_rd_atom);
	print_ibv_atomic_cap(pda->atomic_cap);
	printf("\t max_ee : 0x%x\n",pda->max_ee);
	printf("\t max_rdd : 0x%x\n",pda->max_rdd);
	printf("\t max_mw : 0x%x\n",pda->max_mw);
	printf("\t max_raw_ipv6_qp : 0x%x\n",pda->max_raw_ipv6_qp);
	printf("\t max_raw_ethy_qp : 0x%x\n",pda->max_raw_ethy_qp);
	printf("\t max_mcast_grp : 0x%x\n",pda->max_mcast_grp);
	printf("\t max_mcast_qp_attach : 0x%x\n",pda->max_mcast_qp_attach);
	printf("\t max_total_mcast_qp_attach : 0x%x\n",pda->max_total_mcast_qp_attach);
	printf("\t max_ah : 0x%x\n",pda->max_ah);
	printf("\t max_fmr : 0x%x\n",pda->max_fmr);
	printf("\t max_map_per_fmr : 0x%x\n",pda->max_map_per_fmr);
	printf("\t max_srq : 0x%x\n",pda->max_srq);
	printf("\t max_srq_wr : 0x%x\n",pda->max_srq_wr);
	printf("\t max_srq_sge : 0x%x\n",pda->max_srq_sge);
	printf("\t max_pkeys : 0x%x\n",pda->max_pkeys);
	printf("\t local_ca_ack_delay : 0x%x\n",pda->local_ca_ack_delay);
	printf("\t phys_port_cnt : 0x%x\n",pda->phys_port_cnt);

}

void print_ibv_mtu(enum ibv_mtu mtu) {
	switch(mtu) {
		case IBV_MTU_256 : printf("\t\t IBV_MTU_256 \n");break;
		case IBV_MTU_512 : printf("\t\t IBV_MTU_512 \n");break;
		case IBV_MTU_1024: printf("\t\t IBV_MTU_1024\n");break;
		case IBV_MTU_2048: printf("\t\t IBV_MTU_2048\n");break;
		case IBV_MTU_4096: printf("\t\t IBV_MTU_4096\n");break;
		default : printf("unknow MTU size %d\n",mtu);
	}
}

void print_ibv_port_state(enum ibv_port_state state) {
	switch(state) {
		case IBV_PORT_NOP     :  printf("IBV_PORT_NOP\n");break;
		case IBV_PORT_DOWN    :  printf("IBV_PORT_DOWN\n");break;
		case IBV_PORT_INIT    :  printf("IBV_PORT_INIT\n");break;
		case IBV_PORT_ARMED   :  printf("IBV_PORT_ARMED\n");break;
		case IBV_PORT_ACTIVE  :  printf("IBV_PORT_ACTIVE\n");break;
		case IBV_PORT_ACTIVE_DEFER :  printf("IBV_PORT_ACTIVE_DEFER\n");break;
		default : printf("unknow port state %d\n",state);
	}
}

void get_ibv_link_layer_str(uint8_t link_layer,char * res) {
	switch (link_layer) {
		case IBV_LINK_LAYER_UNSPECIFIED : sprintf(res,"%s","unspecified"); break;
		case IBV_LINK_LAYER_INFINIBAND  : sprintf(res,"%s","infiniband");break;
		case IBV_LINK_LAYER_ETHERNET    : sprintf(res,"%s","ethernet");break;
		default: assert(0);
	}
}

void print_ibv_port_attr(struct ibv_port_attr * ppa) {
	char res[1024];
	print_ibv_port_state(ppa->state);
	printf("\t port state : %s\n",ibv_port_state_str(ppa->state));
	printf("\t max_mtu : ");
	print_ibv_mtu(ppa->max_mtu);
	printf("\t active_mtu : ");
	print_ibv_mtu(ppa->active_mtu);
	printf("\t gid_tbl_len : %d\n",ppa->gid_tbl_len);
	//print_port_cap_flags(ppa->port_cap_flags);
	printf("\t max_msg_sz : %d\n",ppa->max_msg_sz);
	printf("\t bad_pkey_cntr : %d\n",ppa->bad_pkey_cntr);
	printf("\t qkey_viol_cntr : %d\n",ppa->qkey_viol_cntr);
	printf("\t pkey_tbl_len : %d\n",ppa->pkey_tbl_len);
	printf("\t lid : %d\n",ppa->lid);
	printf("\t sm_lid : %d\n",ppa->sm_lid);
	printf("\t lmc : %d\n",ppa->lmc);
	printf("\t max_vl_num : %d\n",ppa->max_vl_num);
	printf("\t sm_sl : %d\n",ppa->sm_sl);
	printf("\t subnet_timeout : %d\n",ppa->subnet_timeout);
	printf("\t init_type_reply : %d\n",ppa->init_type_reply);
	printf("\t active_width : %d\n",ppa->active_width);
	printf("\t active_speed : %d\n",ppa->active_speed);
	printf("\t phys_state : %d\n",ppa->phys_state);
	get_ibv_link_layer_str(ppa->link_layer,res);
	printf("\t link_layer : %s\n",res);
}

void print_ibv_device(struct ibv_device * pdev) {
	print_ibv_node_type(pdev->node_type);
	print_ibv_transport_type(pdev->transport_type);
	printf("Name of underlying kernel IB device : %s\n",pdev->name);
	const char * pname = ibv_get_device_name(pdev);
	assert(strcmp(pname,pdev->name)==0);
	printf("Name of uverbs device : %s\n",pdev->dev_name);
	printf("Path to infiniband_verbs class device in sysfs: %s\n",pdev->dev_path);
	printf("Path to infiniband class device in sysfs : %s\n",pdev->ibdev_path);
	printf("guid : 0x%lx\n",ibv_get_device_guid(pdev));
}

void print_ibv_context(struct ibv_device * pdev,struct ibv_context * pctx) {
		//for context
		assert(pdev==pctx->device);
		struct ibv_device_attr dev_attr;
		ibv_query_device(pctx,&dev_attr);
		printf("\t**** Dev attr ****\n");
		print_ibv_device_attribute(&dev_attr);

		//for port
		for(int i=1;i<=dev_attr.phys_port_cnt;i++) {
			printf("\t\t**** port attr ****\n");
			struct ibv_port_attr pa;
			int resp = ibv_query_port(pctx,i,&pa);
			if(resp) {
				fprintf(stderr,"Error %d : faile to query port %d in dev %s\n",resp,i,pdev->name);
			}
			print_ibv_port_attr(&pa);
		}
}

void work(struct ibv_device * pActiveDev) {
	struct ibv_context * pctx = ibv_open_device(pActiveDev);
	
	{
		//allocate pd
		struct ibv_pd * ppd;
		ppd = ibv_alloc_pd(pctx);
		assert(ppd);
		assert(ppd->context == pctx);

		{
			//memory alloc
			int bufsize = 1024;
			void * pbuf = malloc(bufsize);

			{
				//working on memory registering
				int perm = IBV_ACCESS_LOCAL_WRITE | IBV_ACCESS_REMOTE_WRITE | IBV_ACCESS_REMOTE_READ | IBV_ACCESS_REMOTE_ATOMIC | IBV_ACCESS_MW_BIND | IBV_ACCESS_ZERO_BASED;
				struct ibv_mr * pmr = ibv_reg_mr(ppd,pbuf,bufsize,perm);

				
				
				int resMr = ibv_dereg_mr(pmr);
				assert(resMr==0);
			}

			free(pbuf);
		}

		int res1 = ibv_dealloc_pd(ppd);
		assert(res1==0);
	}

	ibv_close_device(pctx);
}

int main(int argc, char** argv) {
	int num_dev;
	struct ibv_device ** ppdev;

	ppdev = ibv_get_device_list(&num_dev);
	printf("num_dev %d\n",num_dev);
	
	//the list of dev
	//for(int i=num_dev-1;i>=0;i--) {
	//	printf("============= Dev %s ===============\n",ppdev[i]->name);
	//	print_ibv_device(ppdev[i]);

	//	struct ibv_context * pctx = ibv_open_device(ppdev[i]);
	//	print_ibv_context(ppdev[i],pctx);
	//	ibv_close_device(pctx);

	//	printf("\n");
	//}

	//allocate context
	int active_dev_num=0;
	work(ppdev[active_dev_num]);
	
	ibv_free_device_list(ppdev);
}

