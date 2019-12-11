#include <stdint.h>
#include <stdlib.h>
#include <cstdio>
#include <arpa/inet.h>
#include "utility.h"

int getData(const uint8_t *packet, size_t index)
{
    return (int)packet[index] << 8 | (int)packet[index + 1];
}

void writeData(uint8_t *packet, size_t index, int x)
{
    packet[index] = x >> 8;
    packet[index + 1] = x & (1 << 8) - 1;
}

uint8_t reverse(uint8_t b)
{
    b = (b & 0xF0) >> 4 | (b & 0x0F) << 4;
    b = (b & 0xCC) >> 2 | (b & 0x33) << 2;
    b = (b & 0xAA) >> 1 | (b & 0x55) << 1;
    return b;
}

void out(unsigned x, int n)
{
    while (n--)
        putchar('0' ^ x >> n & 1);
}

void output(uint32_t x)
{
    for (int i = 32; i--;)
        putchar('0' ^ x >> i & 1);
}

uint32_t calc(uint32_t addr, int len)
{
    return htonl(addr) >> 32 - len;
}

uint16_t IPChecksum(uint8_t *packet) {
    uint32_t n = (packet[0] & (1 << 4) - 1) << 2;

    uint32_t x = 0;
    for (uint32_t i = 0; i < n; i += 2)
        if (i != 10)
            x += getData(packet, i);
    while (x >= 1 << 16)
        x = (x & (1 << 16) - 1) + (x >> 16);
    
    return ~x & (1 << 16) - 1;
}