# pc1-r1-r2-r3-pc2
# create namespaces. Names follow the convention of documentation of Router Lab
ip netns add pc1
ip netns add r1
ip netns add r2
ip netns add r3
ip netns add pc2
# link the virtual ethernet interfaces
# specifically, the ip of interfaces of [r2] are omitted.
# link [pc1] and [r1]
ip link add veth-pc1-r1 type veth peer name veth-r1-pc1
ip link set veth-pc1-r1 netns pc1
ip link set veth-r1-pc1 netns r1
ip netns exec pc1 ip link set veth-pc1-r1 up
ip netns exec pc1 ip addr add 192.168.1.2/24 dev veth-pc1-r1
ip netns exec r1 ip link set veth-r1-pc1 up
ip netns exec r1 ip addr add 192.168.1.1/24 dev veth-r1-pc1
# link [r1] and [r2]
ip link add veth-r1-r2 type veth peer name veth-r2-r1
ip link set veth-r1-r2 netns r1
ip link set veth-r2-r1 netns r2
ip netns exec r1 ip link set veth-r1-r2 up
ip netns exec r1 ip addr add 192.168.3.1/24 dev veth-r1-r2
ip netns exec r2 ip link set veth-r2-r1 up
# link [r2] and [r3]
ip link add veth-r2-r3 type veth peer name veth-r3-r2
ip link set veth-r2-r3 netns r2
ip link set veth-r3-r2 netns r3
ip netns exec r2 ip link set veth-r2-r3 up
ip netns exec r3 ip link set veth-r3-r2 up
ip netns exec r3 ip addr add 192.168.4.2/24 dev veth-r3-r2
# link [r3] and [pc2]
ip link add veth-r3-pc2 type veth peer name veth-pc2-r3
ip link set veth-r3-pc2 netns r3
ip link set veth-pc2-r3 netns pc2
ip netns exec r3 ip link set veth-r3-pc2 up
ip netns exec r3 ip addr add 192.168.5.2/24 dev veth-r3-pc2
ip netns exec pc2 ip link set veth-pc2-r3 up
ip netns exec pc2 ip addr add 192.168.5.1/24 dev veth-pc2-r3
# add needed route entry
ip netns exec pc1 ip route add default via 192.168.1.1 dev veth-pc1-r1
ip netns exec pc2 ip route add default via 192.168.5.2 dev veth-pc2-r3
# configure forwarding
ip netns exec r1 sysctl -w net.ipv4.ip_forward=1
ip netns exec r2 sysctl -w net.ipv4.ip_forward=0
ip netns exec r3 sysctl -w net.ipv4.ip_forward=1

# pc1-r1-r2-pc2
# # create namespaces. Names follow the convention of documentation of Router Lab
# ip netns add pc1
# ip netns add r1
# ip netns add r2
# ip netns add pc2
# # link the virtual ethernet interfaces
# # specifically, the ip of interfaces of [r2] are omitted.
# # link [pc1] and [r1]
# ip link add veth-pc1-r1 type veth peer name veth-r1-pc1
# ip link set veth-pc1-r1 netns pc1
# ip link set veth-r1-pc1 netns r1
# ip netns exec pc1 ip link set veth-pc1-r1 up
# ip netns exec pc1 ip addr add 192.168.1.2/24 dev veth-pc1-r1
# ip netns exec r1 ip link set veth-r1-pc1 up
# ip netns exec r1 ip addr add 192.168.1.1/24 dev veth-r1-pc1
# # link [r1] and [r2]
# ip link add veth-r1-r2 type veth peer name veth-r2-r1
# ip link set veth-r1-r2 netns r1
# ip link set veth-r2-r1 netns r2
# ip netns exec r1 ip link set veth-r1-r2 up
# ip netns exec r1 ip addr add 192.168.3.1/24 dev veth-r1-r2
# ip netns exec r2 ip link set veth-r2-r1 up
# ip netns exec r2 ip addr add 192.168.3.2/24 dev veth-r2-r1
# # link [r2] and [pc2]
# ip link add veth-r2-pc2 type veth peer name veth-pc2-r2
# ip link set veth-r2-pc2 netns r2
# ip link set veth-pc2-r2 netns pc2
# ip netns exec r2 ip link set veth-r2-pc2 up
# ip netns exec r2 ip addr add 192.168.4.1/24 dev veth-r2-pc2
# ip netns exec pc2 ip link set veth-pc2-r2 up
# ip netns exec pc2 ip addr add 192.168.4.2/24 dev veth-pc2-r2
# # add needed route entry
# ip netns exec pc1 ip route add 192.168.4.0/24 via 192.168.1.1 dev veth-pc1-r1
# ip netns exec pc2 ip route add 192.168.1.0/24 via 192.168.4.1 dev veth-pc2-r2
# # configure forwarding
# ip netns exec r1 sysctl -w net.ipv4.ip_forward=1
# ip netns exec r2 sysctl -w net.ipv4.ip_forward=1

# pc1-r1-pc2
# # create namespaces. Names follow the convention of documentation of Router Lab
# ip netns add pc1
# ip netns add r1
# ip netns add pc2
# # link [pc1] and [r1]
# ip link add veth-pc1-r1 type veth peer name veth-r1-pc1
# ip link set veth-pc1-r1 netns pc1
# ip link set veth-r1-pc1 netns r1
# ip netns exec pc1 ip link set veth-pc1-r1 up
# ip netns exec pc1 ip addr add 192.168.1.2/24 dev veth-pc1-r1
# ip netns exec r1 ip link set veth-r1-pc1 up
# ip netns exec r1 ip addr add 192.168.1.1/24 dev veth-r1-pc1
# # link [r1] and [pc2]
# ip link add veth-r1-pc2 type veth peer name veth-pc2-r1
# ip link set veth-r1-pc2 netns r1
# ip link set veth-pc2-r1 netns pc2
# ip netns exec r1 ip link set veth-r1-pc2 up
# ip netns exec r1 ip addr add 192.168.3.1/24 dev veth-r1-pc2
# ip netns exec pc2 ip link set veth-pc2-r1 up
# ip netns exec pc2 ip addr add 192.168.3.2/24 dev veth-pc2-r1
# # add needed route entry
# ip netns exec pc1 ip route add 192.168.3.0/24 via 192.168.1.1 dev veth-pc1-r1
# ip netns exec pc2 ip route add 192.168.1.0/24 via 192.168.3.1 dev veth-pc2-r1

# Copied from the document of Router Lab
# ip netns add net0 # 创建名为 "net0" 的 namespace
# ip netns add net1
# ip link add veth-net0 type veth peer name veth-net1 # 创建一对相互连接的 veth pair
# ip link set veth-net0 netns net0 # 将 veth 一侧加入到一个 namespace 中
# ip link set veth-net1 netns net1 # 配置 veth 另一侧
# ip netns exec net0 ip link set veth-net0 up
# ip netns exec net0 ip addr add 10.1.1.1/24 dev veth-net0 # 给 veth 一侧配上 ip 地址
# ip netns exec net1 ip link set veth-net1 up
# ip netns exec net1 ip addr add 10.1.1.2/24 dev veth-net1