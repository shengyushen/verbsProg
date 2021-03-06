#include "print_rdma.h"
#include "rdma_common.h"

void print_ibv_wc_flags(int wc_flags) {
	if(wc_flags | IBV_WC_GRH) printf("IBV_WC_GRH\n");
	if(wc_flags | IBV_WC_WITH_IMM) printf("IBV_WC_WITH_IMM\n");
	if(wc_flags | IBV_WC_WITH_INV) printf("IBV_WC_WITH_INV\n");
}

void print_ibv_opcode(enum ibv_wc_opcode op) {
	switch (op) {
		case IBV_WC_SEND : printf("IBV_WC_SEND");break;
		case IBV_WC_RDMA_WRITE : printf("IBV_WC_RDMA_WRITE");break;
		case IBV_WC_RDMA_READ : printf("IBV_WC_RDMA_READ");break;
		case IBV_WC_COMP_SWAP : printf("IBV_WC_COMP_SWAP");break;
		case IBV_WC_FETCH_ADD : printf("IBV_WC_FETCH_ADD");break;
		case IBV_WC_BIND_MW : printf("IBV_WC_BIND_MW");break;
		case IBV_WC_LOCAL_INV : printf("IBV_WC_LOCAL_INV");break;
		case IBV_WC_RECV : printf("IBV_WC_RECV");break;
		case IBV_WC_RECV_RDMA_WITH_IMM : printf("IBV_WC_RECV_RDMA_WITH_IMM");break;
		default : assert(0);
	}
	printf("\n");
}

void print_ibv_wc_status(enum ibv_wc_status st) {
	switch(st) {
		case IBV_WC_SUCCESS:								printf("IBV_WC_SUCCESS");break;
		case IBV_WC_LOC_LEN_ERR:            printf("IBV_WC_LOC_LEN_ERR");break;
		case IBV_WC_LOC_QP_OP_ERR:          printf("IBV_WC_LOC_QP_OP_ERR");break;
		case IBV_WC_LOC_EEC_OP_ERR:         printf("IBV_WC_LOC_EEC_OP_ERR");break;
		case IBV_WC_LOC_PROT_ERR:           printf("IBV_WC_LOC_PROT_ERR");break;
		case IBV_WC_WR_FLUSH_ERR:           printf("IBV_WC_WR_FLUSH_ERR");break;
		case IBV_WC_MW_BIND_ERR:            printf("IBV_WC_MW_BIND_ERR");break;
		case IBV_WC_BAD_RESP_ERR:           printf("IBV_WC_BAD_RESP_ERR");break;
		case IBV_WC_LOC_ACCESS_ERR:         printf("IBV_WC_LOC_ACCESS_ERR");break;
		case IBV_WC_REM_INV_REQ_ERR:        printf("IBV_WC_REM_INV_REQ_ERR");break;
		case IBV_WC_REM_ACCESS_ERR:         printf("IBV_WC_REM_ACCESS_ERR");break;
		case IBV_WC_REM_OP_ERR:             printf("IBV_WC_REM_OP_ERR");break;
		case IBV_WC_RETRY_EXC_ERR:          printf("IBV_WC_RETRY_EXC_ERR");break;
		case IBV_WC_RNR_RETRY_EXC_ERR:      printf("IBV_WC_RNR_RETRY_EXC_ERR");break;
		case IBV_WC_LOC_RDD_VIOL_ERR:       printf("IBV_WC_LOC_RDD_VIOL_ERR");break;
		case IBV_WC_REM_INV_RD_REQ_ERR:     printf("IBV_WC_REM_INV_RD_REQ_ERR");break;
		case IBV_WC_REM_ABORT_ERR:          printf("IBV_WC_REM_ABORT_ERR");break;
		case IBV_WC_INV_EECN_ERR:           printf("IBV_WC_INV_EECN_ERR");break;
		case IBV_WC_INV_EEC_STATE_ERR:      printf("IBV_WC_INV_EEC_STATE_ERR");break;
		case IBV_WC_FATAL_ERR:              printf("IBV_WC_FATAL_ERR");break;
		case IBV_WC_RESP_TIMEOUT_ERR:       printf("IBV_WC_RESP_TIMEOUT_ERR");break;
		case IBV_WC_GENERAL_ERR:					  printf("IBV_WC_GENERAL_ERR");break;
		default : 
			printf("ibv_wc_status is %d\n",st);
			printf("ibv_wc_status str is %s\n",ibv_wc_status_str(st));
			assert(0);
	}
	printf("\n");
}

void print_ibv_wc(struct ibv_wc * pwc) {
	printf("wr_id is %ld\n",pwc->wr_id);
	print_ibv_wc_status(pwc->status);
	print_ibv_opcode(pwc->opcode);
	printf("vendor_err %d\n",pwc->vendor_err);
	printf("byte_len %d\n",pwc->byte_len);
	printf("imm_data %d\n",pwc->imm_data);
	printf("qp_num %d\n",pwc->qp_num);
	printf("src_qp %d\n",pwc->src_qp);
	print_ibv_wc_flags(pwc->wc_flags);
	printf("pkey_index %d\n",pwc->pkey_index);
	printf("slid %d\n",pwc->slid);
	printf("sl %d\n",pwc->sl);
	printf("dlid_path_bits %d\n",pwc->dlid_path_bits);
}


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

void print_tm_cap_str(uint32_t ui) {
	if(ui | IBV_EXP_TM_CAP_RC)
		printf("IBV_EXP_TM_CAP_RC\n");
	
	if(ui | IBV_EXP_TM_CAP_DC)		
		printf("IBV_EXP_TM_CAP_DC\n");

}

void print_ibv_exp_device_attribute(struct ibv_exp_device_attr * pexp_data_attr) {
	//there are too many flags
	//only tag matching related is tried
	if((pexp_data_attr -> comp_mask | IBV_EXP_DEVICE_ATTR_TM_CAPS))  {
		printf("IBV_EXP_DEVICE_ATTR_TM_CAPS\n");
		printf("max_rndv_hdr_size %d\n",(pexp_data_attr -> tm_caps).max_rndv_hdr_size);
		printf("max_num_tags %d\n", (pexp_data_attr -> tm_caps).max_num_tags);
		print_tm_cap_str((pexp_data_attr -> tm_caps).capability_flags);
		printf("max_ops %d\n", (pexp_data_attr -> tm_caps).max_ops);
		printf("max_sge %d\n", (pexp_data_attr -> tm_caps).max_sge);
	}
}


void print_ibv_context(struct ibv_device * pdev,struct ibv_context * pctx) {
		//for context
		assert(pdev==pctx->device);
		struct ibv_device_attr dev_attr;
		ibv_query_device(pctx,&dev_attr);
		printf("\t**** Dev attr ****\n");
		print_ibv_device_attribute(&dev_attr);

		printf("\t**** Exp Dev attr ****\n");
		struct ibv_exp_device_attr exp_dev_attr;
		ibv_exp_query_device(pctx,&exp_dev_attr);
		print_ibv_exp_device_attribute(&exp_dev_attr);

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

