
#include "math.h"

#define BITS_PER_BYTE 8
#define BYTES_IN_128_BITS (128 / BITS_PER_BYTE)
#define LEFT_MOST_BYTE (BYTES_IN_128_BITS - 1)


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

// Detailed breakdown of the math
//  + lookup reverse of bottom nibble
//  |       + grab bottom nibble
//  |       |        + move bottom result into top nibble
//  |       |        |     + combine the bottom and top results
//  |       |        |     | + lookup reverse of top nibble
//  |       |        |     | |       + grab top nibble
//  V       V        V     V V       V
// (lookup[n&0b1111] << 4) | lookup[n>>4]




void print_region(uint8_t *region){

	printf("Region:\n");

	for (uint8_t i = 0; i < BYTES_IN_128_BITS; i++){
		printf("%02X ", region[i]);
	}
	printf("\n");

}



/* Little-little endian implementation */
void galois_128_mult_lle(uint8_t *region_x, uint8_t *region_y, uint8_t *region_z){


	// Declare vars
	uint8_t i;
	uint8_t j;
	uint8_t region_v[BYTES_IN_128_BITS];

	// Initialize
	uint8_t was_bit_high = 0;
	// Z <- 0
	memset(region_z, 0x0, BYTES_IN_128_BITS);
	// V <- X
	memcpy(region_v, region_x, BYTES_IN_128_BITS);

	// |B0|B1|B2|..|Bn|
	for (i = 0; i < BYTES_IN_128_BITS; i++){

		// |b0|b1|b2|..|b7|
		for (j = BITS_PER_BYTE; j > 0; j--){

			// If Yi = 1
			if ( (region_y[i] >> (j-1)) & 0x1 ){

				//printf("true when i == %d and j == %d\n", i, (j-1));

				// Z <- Z Xor V
				((uint64_t*)region_z)[0] ^= ((uint64_t*)region_v)[0];
				((uint64_t*)region_z)[1] ^= ((uint64_t*)region_v)[1];

			}

			// If V127 = 1 --> BYTE 15 [b120, b121, ..., b127]
			if ( region_v[15] & 0x01 ){
				// Xor with R
				region_v[0] |= 0b11100001; // 1 + x + x^2 + x^7
			}

			// Before shifting, reverse bits in all bytes!
			region_v[0] = _reverse(region_v[0]);
			region_v[1] = _reverse(region_v[1]);
			region_v[2] = _reverse(region_v[2]);
			region_v[3] = _reverse(region_v[3]);
			region_v[4] = _reverse(region_v[4]);
			region_v[5] = _reverse(region_v[5]);
			region_v[6] = _reverse(region_v[6]);
			region_v[7] = _reverse(region_v[7]);
			region_v[8] = _reverse(region_v[8]);
			region_v[9] = _reverse(region_v[9]);
			region_v[10] = _reverse(region_v[10]);
			region_v[11] = _reverse(region_v[11]);
			region_v[12] = _reverse(region_v[12]);
			region_v[13] = _reverse(region_v[13]);
			region_v[14] = _reverse(region_v[14]);
			region_v[15] = _reverse(region_v[15]);

			// Left shift V
			was_bit_high = region_v[7] & 0x80; // --> BYTE 7 [b63, b62, ..., b56]
			((uint64_t*)region_v)[0] = ((uint64_t*)region_v)[0] << 1;
			((uint64_t*)region_v)[1] = ((uint64_t*)region_v)[1] << 1;
			if ( was_bit_high )
				region_v[8] |= 0x01; // --> BYTE 8 [b71, b70, ..., b64]

			// Now, re-reverse bits in all bytes!
			region_v[0] = _reverse(region_v[0]);
			region_v[1] = _reverse(region_v[1]);
			region_v[2] = _reverse(region_v[2]);
			region_v[3] = _reverse(region_v[3]);
			region_v[4] = _reverse(region_v[4]);
			region_v[5] = _reverse(region_v[5]);
			region_v[6] = _reverse(region_v[6]);
			region_v[7] = _reverse(region_v[7]);
			region_v[8] = _reverse(region_v[8]);
			region_v[9] = _reverse(region_v[9]);
			region_v[10] = _reverse(region_v[10]);
			region_v[11] = _reverse(region_v[11]);
			region_v[12] = _reverse(region_v[12]);
			region_v[13] = _reverse(region_v[13]);
			region_v[14] = _reverse(region_v[14]);
			region_v[15] = _reverse(region_v[15]);



		}

	}


}




void galois_128_mult_lbe(uint8_t *region_x, uint8_t *region_y, uint8_t *region_z){


	// Declare vars
	uint8_t i;
	uint8_t j;
	uint8_t region_v[BYTES_IN_128_BITS];

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

				//printf("true when i == %d and j == %d\n", i, (j-1));

				// Z <- Z Xor V
				((uint64_t*)region_z)[0] ^= ((uint64_t*)region_v)[0];
				((uint64_t*)region_z)[1] ^= ((uint64_t*)region_v)[1];

			}

			// If V127 = 1 --> BYTE 15 [b120, b121, ..., b127]
			if ( region_v[15] & 0x01 ){
				// Xor with R
				region_v[0] |= 0b10000111; // 1 + x + x^2 + x^7
			}

			// Left shift V
			was_bit_high = region_v[7] & 0x80; // --> BYTE 7 [b63, b62, ..., b56]
			((uint64_t*)region_v)[0] = ((uint64_t*)region_v)[0] << 1;
			((uint64_t*)region_v)[1] = ((uint64_t*)region_v)[1] << 1;
			if ( was_bit_high )
				region_v[8] |= 0x01; // --> BYTE 8 [b71, b70, ..., b64]



		}

	}


}


uint32_t _bswap32(uint32_t a){

  a = ((a & 0x000000FF) << 24) |
      ((a & 0x0000FF00) <<  8) |
      ((a & 0x00FF0000) >>  8) |
      ((a & 0xFF000000) >> 24);
  return a;

}

void add(uint8_t *block, uint8_t *block2){

	((uint32_t*)block)[0] ^= ((uint32_t*)block2)[0];
	((uint32_t*)block)[1] ^= ((uint32_t*)block2)[1];
	((uint32_t*)block)[2] ^= ((uint32_t*)block2)[2];
	((uint32_t*)block)[3] ^= ((uint32_t*)block2)[3];

}


