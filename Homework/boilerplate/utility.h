#ifndef __UTILITY_H__
#define __UTILITY_H__

#include <stdint.h>
#include <stdlib.h>
#include <cstdio>
#include "router.h"

int getData(const uint8_t *packet, size_t index);
void writeData(uint8_t *packet, size_t index, int x);
uint8_t reverse(uint8_t b);
void out(unsigned x, int n);
void output(uint32_t x);
uint32_t calc(uint32_t addr, int len);
uint16_t IPChecksum(uint8_t *packet);

#endif