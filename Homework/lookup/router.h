#ifndef __ROUTER_H__
#define __ROUTER_H__

#include <stdint.h>
#include <inttypes.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstdio>

// 路由表的一项
typedef struct {
    uint32_t addr; // 大端序，IPv4 地址
    uint32_t len; // 小端序，前缀长度
    uint32_t if_index; // 小端序，出端口编号
    uint32_t nexthop; // 大端序，下一跳的 IPv4 地址
    uint32_t metric; // 大端序，值域为[1, 16].

    void print() {
      printf("RoutingTableEntry {addr = %s", inet_ntoa(in_addr{addr}));
      printf(", len = %" PRIu32 ", if_index = %" PRIu32, len, if_index);
      printf(", nexthop = %s", inet_ntoa(in_addr{nexthop}));
      printf(", metric = %s}", inet_ntoa(in_addr{metric}));
    }
} RoutingTableEntry;

#endif