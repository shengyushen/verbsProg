#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <unistd.h>

#include <infiniband/verbs.h>
#include <infiniband/verbs_exp.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

void print_ibv_wc(struct ibv_wc * pwd);
void print_ibv_opcode(enum ibv_wc_opcode op);
void print_ibv_wc_status(enum ibv_wc_status st);
void print_ibv_node_type(enum ibv_node_type node_type);
void print_ibv_transport_type(enum ibv_transport_type tp);
void print_ibv_atomic_cap(enum ibv_atomic_cap acap);
void print_ibv_device_cap_flags(enum ibv_device_cap_flags dcf);
void print_ibv_device_attribute(struct ibv_device_attr * pda);
void print_ibv_mtu(enum ibv_mtu mtu);
void print_ibv_port_state(enum ibv_port_state state);
void get_ibv_link_layer_str(uint8_t link_layer,char * res);
void print_ibv_port_attr(struct ibv_port_attr * ppa);
void print_ibv_device(struct ibv_device * pdev);
void print_tm_cap_str(uint32_t ui);
void print_ibv_exp_device_attribute(struct ibv_exp_device_attr * pexp_data_attr);
void print_ibv_context(struct ibv_device * pdev,struct ibv_context * pctx);
