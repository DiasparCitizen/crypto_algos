
#ifndef MATH_H_
#define MATH_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

void galois_128_mult_lle(uint8_t *region_x, uint8_t *region_y, uint8_t *region_z);
void galois_128_mult_lbe(uint8_t *region_x, uint8_t *region_y, uint8_t *region_z);
void print_region(uint8_t *region);

uint32_t _bswap32(uint32_t a);
void add(uint8_t *block, uint8_t *block2);





#endif /* MATH_H_ */
