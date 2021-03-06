/*
 * * ** Copyright (C) 2015 Westlab, All Rights Reserved.
 * */

#ifndef VLAN_HDR_H_
#define VLAN_HDR_H_

struct vlan_ethhdr {
    u_int8_t  ether_dhost[ETH_ALEN];  /* destination eth addr */
    u_int8_t  ether_shost[ETH_ALEN];  /* source ether addr    */
    u_int16_t          h_vlan_proto;
    u_int16_t          h_vlan_TCI;
    u_int16_t ether_type;
};

#endif  // VLAN_HDR_H_
