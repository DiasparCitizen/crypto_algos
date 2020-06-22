
#include "math.h"

#define BITS_PER_BYTE 8
#define BYTES_IN_128_BITS (128 / BITS_PER_BYTE)
#define LEFT_MOST_BYTE (BYTES_IN_128_BITS - 1)

// Macros

#define _reverse(n) (lookup[n&0b1111] << 4) | lookup[n>>4]

//Index 1==0b0001 => 0b1000
//Index 7==0b0111 => 0b1110
//etc
static unsigned char lookup[16] = {
0x0, 0x8, 0x4, 0xc, 0x2, 0xa, 0x6, 0xe,
0x1, 0x9, 0x5, 0xd, 0x3, 0xb, 0x7, 0xf, };

uint8_t reverse(uint8_t n) {
   // Reverse the top and bottom nibble then swap them.
   return (lookup[n&0b1111] << 4) | lookup[n>>4];
}

void galois_128_mult(uint8_t *region_x, uint8_t *region_y, uint8_t *region_z){

	if (0){
		return galois_128_mult_lle_reverse(region_x, region_y, region_z);
	}else{
		return galois_128_mult_lle(region_x, region_y, region_z);
	}

}

// galois_128_mult_lle(g_sub_i, H, aux);
void galois_128_mult_lle(uint8_t *region_x, uint8_t *region_y, uint8_t *region_z){

	// Declare vars
	uint8_t byteIdx, bitIdx, i;
	uint8_t region_v[BYTES_IN_128_BITS];
	uint8_t was127set, was_bit_high, aux;

#define R_MASK 0b11100001

	// Z <- 0, V <- X
	memset(region_z, 0x0, BYTES_IN_128_BITS);
	memcpy(region_v, region_x, BYTES_IN_128_BITS);

	for (byteIdx = 0; byteIdx < BYTES_IN_128_BITS; byteIdx++){

		for (bitIdx = 0; bitIdx < 8; bitIdx++){

			if ( (region_y[byteIdx] >> (7 - bitIdx)) & 0x1 ){
				// Z = Z xor V
				((uint64_t*)region_z)[0] ^= ((uint64_t*)region_v)[0];
				((uint64_t*)region_z)[1] ^= ((uint64_t*)region_v)[1];
			}

			// Save bit 127, since it will be lost after the rightshift op
			was127set = region_v[BYTES_IN_128_BITS - 1] & 0x1;

			// rightshift(V) (Always done)
			was_bit_high = region_v[0] & 0x1;
			region_v[0] >>= 1;

			for (i = 1; i < BYTES_IN_128_BITS; i++){
				aux = region_v[i] & 0x1;
				region_v[i] >>= 1;
				if (was_bit_high) region_v[i] |= 0x80;
				was_bit_high = aux;
			}

			// If V.127 == 0, rightshift(V)
			// If V.127 == 1, rightshift(V) xor R
			if ( was127set ) region_v[0] ^= R_MASK;

		}

	}

#undef R_MASK

	return;

}

// galois_128_mult(g_sub_i, H, aux);
void galois_128_mult_lle_reverse(uint8_t *region_x, uint8_t *region_y, uint8_t *region_z){

	// Declare vars
	uint8_t i;
	uint8_t j;
	uint8_t region_v[BYTES_IN_128_BITS];

#define R_MASK 0b10000111;

	// Reverse input
	for (i = 0; i < BYTES_IN_128_BITS; i++){
		region_x[i] = _reverse(region_x[i]);
		region_y[i] = _reverse(region_y[i]);
	}

	// Initialize
	uint8_t was_bit_high = 0;
	// Z <- 0
	memset(region_z, 0x0, BYTES_IN_128_BITS);
	// V <- X
	memcpy(region_v, region_x, BYTES_IN_128_BITS);

	// |B0|B1|B2|..|Bn|
	for (i = 0; i < BYTES_IN_128_BITS; i++){

		// |b0|b1|b2|..|b7|
		for (j = 0; j < BITS_PER_BYTE; j++){

			// If Yi = 1
			if ( (region_y[i] >> j) & 0x1 ){
				// Z <- Z Xor V
				((uint64_t*)region_z)[0] ^= ((uint64_t*)region_v)[0];
				((uint64_t*)region_z)[1] ^= ((uint64_t*)region_v)[1];
			}

			uint8_t was127set = region_v[15] & 0x80;

			// Left shift V (alawys done)
			was_bit_high = region_v[7] & 0x80; // --> BYTE 7 [b63, b62, ..., b56]
			((uint64_t*)region_v)[0] = ((uint64_t*)region_v)[0] << 1;
			((uint64_t*)region_v)[1] = ((uint64_t*)region_v)[1] << 1;
			if ( was_bit_high )
				region_v[8] |= 0x01; // --> BYTE 8 [b71, b70, ..., b64]

			// If V127 = 1 --> BYTE 15 [b120, b121, ..., b127]
			if ( was127set ){
				// Xor with R
				region_v[0] ^= R_MASK; // 1 + x + x^2 + x^7
			}

		}

	}

	// Re-reverse
	for (i = 0; i < BYTES_IN_128_BITS; i++){
		region_x[i] = _reverse(region_x[i]);
		region_y[i] = _reverse(region_y[i]);
		region_z[i] = _reverse(region_z[i]);
	}

#undef R_MASK

}

uint32_t ipow(uint32_t base, uint32_t exp){
	uint32_t result = 1;
    for (;;)
    {
        if (exp & 1)
            result *= base;
        exp >>= 1;
        if (!exp)
            break;
        base *= base;
    }

    return result;
}

uint64_t _bswap64(uint64_t val){

    val = ((val << 8) & 0xFF00FF00FF00FF00ULL) | ((val >> 8) & 0x00FF00FF00FF00FFULL);
    val = ((val << 16) & 0xFFFF0000FFFF0000ULL) | ((val >> 16) & 0x0000FFFF0000FFFFULL);
    return (val << 32) | ((val >> 32) & 0xFFFFFFFFULL);

}

uint32_t _bswap32(uint32_t a){

  a = ((a & 0x000000FF) << 24) |
      ((a & 0x0000FF00) <<  8) |
      ((a & 0x00FF0000) >>  8) |
      ((a & 0xFF000000) >> 24);
  return a;

}

void reverseBytes(void *start, int size){
    unsigned char *lo = start;
    unsigned char *hi = start + size - 1;
    unsigned char swap;
    while (lo < hi) {
        swap = *lo;
        *lo++ = *hi;
        *hi-- = swap;
    }
}

void xor(uint8_t *block, uint8_t *block2){

	((uint32_t*)block)[0] ^= ((uint32_t*)block2)[0];
	((uint32_t*)block)[1] ^= ((uint32_t*)block2)[1];
	((uint32_t*)block)[2] ^= ((uint32_t*)block2)[2];
	((uint32_t*)block)[3] ^= ((uint32_t*)block2)[3];

}

void xor_bytes(uint8_t *block, uint8_t *block2, uint16_t byte_len){
	for (uint16_t i = 0; i < byte_len; i++){
		block[i] ^= block2[i];
	}
}

void flip_bytes(uint8_t *bytes, uint8_t len){

	for (uint8_t i = 0; i < len; i++){
		bytes[i] = flip_byte(bytes[i]);
	}

}

uint8_t flip_byte(uint8_t b){

	/*uint8_t flipped_byte = 0;

	flipped_byte |= ((*b) & 0x1) << 7;
	flipped_byte |= ((*b) & 0x2) << 5;
	flipped_byte |= ((*b) & 0x4) << 3;
	flipped_byte |= ((*b) & 0x8) << 1;

	flipped_byte |= ((*b) & 0x10) >> 1;
	flipped_byte |= ((*b) & 0x20) >> 3;
	flipped_byte |= ((*b) & 0x40) >> 5;
	flipped_byte |= ((*b) & 0x80) >> 7;*/

	b = (b & 0xF0) >> 4 | (b & 0x0F) << 4;
	b = (b & 0xCC) >> 2 | (b & 0x33) << 2;
	b = (b & 0xAA) >> 1 | (b & 0x55) << 1;
	return b;

}

uint16_t rotl16(uint16_t x, unsigned int n){
  const unsigned int mask = (8*sizeof(x)-1);
  n &= mask;  // avoid undef behaviour with NDEBUG.  0 overhead for most types / compilers
  return (x<<n) | (x>>( (-n)&mask ));
}
