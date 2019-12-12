#include <stdint.h>
#include <inttypes.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstdio>

int main() {
    uint32_t x1 = 0x0101000a, x2 = 0x0102000a;
    printf("%s\n", inet_ntoa(in_addr{x1}));
    printf("%s\n", inet_ntoa(in_addr{x2}));
}