#include "router.h"
#include <stdint.h>
#include <stdlib.h>
#include <bits/stdc++.h>
#include <tuple>
#include <arpa/inet.h>
#include "utility.h"
using namespace std;

/*
  RoutingTable Entry 的定义如下：
  typedef struct {
    uint32_t addr; // 大端序，IPv4 地址
    uint32_t len; // 小端序，前缀长度
    uint32_t if_index; // 小端序，出端口编号
    uint32_t nexthop; // 大端序，下一跳的 IPv4 地址
    uint32_t metric; // 小端序，值域为[1, 16].
  } RoutingTableEntry;

  约定 addr 和 nexthop 以 **大端序** 存储。
  这意味着 1.2.3.4 对应 0x04030201 而不是 0x01020304。
  保证 addr 仅最低 len 位可能出现非零。
  当 nexthop 为零时这是一条直连路由。
  你可以在全局变量中把路由表以一定的数据结构格式保存下来。
*/

// key: <addr, len>, value: <if_index, nexthop, metric>
map<pair<uint32_t, uint32_t>, tuple<uint32_t, uint32_t, uint32_t>> table;

/**
 * @brief 插入/删除一条路由表表项
 * @param insert 如果要插入则为 true ，要删除则为 false
 * @param entry 要插入/删除的表项
 * @param return 路由表的结构是否发生改变
 * 
 * 插入时如果已经存在一条 addr 和 len 都相同的表项，则替换掉原有的。
 * 删除时按照 addr 和 len 匹配。
 */
bool update(bool insert, RoutingTableEntry entry)
{
    auto key = make_pair(calc(entry.addr, entry.len), entry.len);
    if (table.find(key) != table.end()) {
        if (insert) {
            if (entry.metric < get<2>(table[key])) {
                table[key] = make_tuple(entry.nexthop, entry.if_index, entry.metric);
                return true;
            }
        }
        else {
            table.erase(key);
            return true;
        }
    }
    return false;
}

/**
 * @brief 进行一次路由表的查询，按照最长前缀匹配原则
 * @param addr 需要查询的目标地址，大端序
 * @param nexthop 如果查询到目标，把表项的 nexthop 写入
 * @param if_index 如果查询到目标，把表项的 if_index 写入
 * @return 查到则返回 true ，没查到则返回 false
 */
bool query(uint32_t addr, uint32_t *nexthop, uint32_t *if_index)
{
    for (int i = 32; i >= 0; --i)
    {
        auto it = table.find(make_pair(calc(addr, i), i));
        if (it != table.end())
        {
            *nexthop = get<0>(it->second);
            *if_index = get<1>(it->second);
            return true;
        }
    }
    return false;
}

// The interface to send is always needed to consider.
// I'm not sure if this satisfies RFC,
// but it seems reasonable to me.
RipPacket *routingTable(int if_index) {
  RipPacket *p = new RipPacket();
  p->command = 0x2; // Command Response
  p->numEntries = 0;
  for (auto it = table.first(); it != table.end() ; ++it) {
    if (if_index != get<0>(it->value)) {
        p->entries[numEntries++] = {
        // The format of the routing entry
        // key: <addr, len>, value: <if_index, nexthop, metric>
        .addr = (it->key).first,
        .mask = (it->key).second == 0 ? 0: htonl(~((1 << 32 - (it->key).second) - 1)),
        .nexthop = get<1>(it->value),
        .metric = min(get<2>(it->value) + 1, 16)
        };
    }
  }
  return p;
}