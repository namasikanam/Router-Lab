#include <stdint.h>
#include <stdlib.h>
#include <cstdio>

uint8_t reverse(uint8_t b)
{
    b = (b & 0xF0) >> 4 | (b & 0x0F) << 4;
    b = (b & 0xCC) >> 2 | (b & 0x33) << 2;
    b = (b & 0xAA) >> 1 | (b & 0x55) << 1;
    return b;
}
int getData(uint8_t *packet, size_t index)
{
    return (int)packet[index + 1] << 8 | (int)packet[index];
}

/**
 * @brief 进行 IP 头的校验和的验证
 * @param packet 完整的 IP 头和载荷
 * @param len 即 packet 的长度，单位是字节，保证包含完整的 IP 头
 * @return 校验和无误则返回 true ，有误则返回 false
 */
bool validateIPChecksum(uint8_t *packet, size_t len)
{
    // for (int i = 8; i--;)
    //     printf("%d", packet[0] >> i & 1);
    // puts("");

    int n = (packet[0] & (1 << 4) - 1) << 2;

    // printf("n = %d\n", n);

    int x = 0;
    for (int i = 0; i < n; i += 2)
        if (i != 10)
            x += getData(packet, i);
    while (x >= 1 << 16)
        x = (x & (1 << 16) - 1) + (x >> 16);

    // for (int i = 16; i--;)
    //     printf("%d", x >> i & 1);
    // puts("");
    // for (int i = 16; i--;)
    //     printf("%d", getData(packet, 10) >> i & 1);
    // puts("");

    return (~x & (1 << 16) - 1) == getData(packet, 10);
}
