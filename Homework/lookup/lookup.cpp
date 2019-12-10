#include "router.h"
#include <stdint.h>
#include <stdlib.h>
#include <bits/stdc++.h>
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
  } RoutingTableEntry;

  约定 addr 和 nexthop 以 **大端序** 存储。
  这意味着 1.2.3.4 对应 0x04030201 而不是 0x01020304。
  保证 addr 仅最低 len 位可能出现非零。
  当 nexthop 为零时这是一条直连路由。
  你可以在全局变量中把路由表以一定的数据结构格式保存下来。
*/

map<pair<uint32_t, int>, pair<uint32_t, uint32_t>> table;
uint32_t calc(uint32_t addr, int len)
{
    // return len == 32 ? htonl(addr) : htonl(addr) & (1 << len) - 1;
    return htonl(addr) >> 32 - len;
}

/**
 * @brief 插入/删除一条路由表表项
 * @param insert 如果要插入则为 true ，要删除则为 false
 * @param entry 要插入/删除的表项
 * 
 * 插入时如果已经存在一条 addr 和 len 都相同的表项，则替换掉原有的。
 * 删除时按照 addr 和 len 匹配。
 */
void update(bool insert, RoutingTableEntry entry)
{
    if (insert)
    {
        // printf("insert (addr = ");
        // output(htonl(entry.addr));
        // printf("(");
        // output(htonl(entry.addr) & (1 << entry.len) - 1);
        // printf("), len = %d)\n", entry.len);

        table[make_pair(calc(entry.addr, entry.len), entry.len)] = make_pair(entry.nexthop, entry.if_index);
    }
    else
        table.erase(make_pair(calc(entry.addr, entry.len), entry.len));
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
            // printf("Find ");
            // output(addr);
            // printf("(");
            // output(addr & (1 << i) - 1);
            // printf(") at %d\n", i);

            *nexthop = ((it->second).first);
            *if_index = ((it->second).second);
            return true;
        }
    }
    return false;
}
