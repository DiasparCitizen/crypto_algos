
#ifndef MATH_H_
#define MATH_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#define OOOOOOII 0x3
#define OOOOIIII 0xF
#define OOIIIIII 0x3F

void galois_128_mult(uint8_t *region_x, uint8_t *region_y, uint8_t *region_z);
void galois_128_mult_lle_reverse(uint8_t *region_x, uint8_t *region_y, uint8_t *region_z);
void galois_128_mult_lle(uint8_t *region_x, uint8_t *region_y, uint8_t *region_z);
void print_region(uint8_t *region);

uint32_t _bswap32(uint32_t val);
uint64_t _bswap64(uint64_t val);
void xor(uint8_t *block, uint8_t *block2);
void xor_bytes(uint8_t *block, uint8_t *block2, uint16_t len);
uint8_t flip_byte(uint8_t b);
void flip_bytes(uint8_t *bytes, uint8_t len);

uint32_t ipow(uint32_t base, uint32_t exp);
void reverseBytes(void *start, int size);

uint16_t rotl16(uint16_t x, unsigned int n);


#endif /* MATH_H_ */
