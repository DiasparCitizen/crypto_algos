/*
 * DES.c
 *
 *  Created on: 27 feb. 2019
 *      Author: Ich
 */

#include "DES.h"


void initial_permutation(uint8_t * block){

	// Make a copy of the block
	uint8_t block_copy[DES_BLOCK_BYTE_LEN];
	memcpy(block_copy, block, DES_BLOCK_BYTE_LEN);

	int byte_offset = 0;
	int bit_offset = 0;

	for (int i = 0; i < 8; i++){

		for (int j = (DES_BLOCK_BYTE_LEN - 1); j >= 0; j--){

			int src_byte_idx = j > 3 ? (7 - j) : (3 - j);

			// Bits 0, 2, 4, 6 fill bytes from 4 to 7
			// Bits 1, 3, 5, 7 fill bytes from 0 to 3
			byte_offset = (i % 2) == 0 ? 4 : 0;

			// 0-1 / 2 = 0
			// 2-3 / 2 = 1
			// 4-5 / 2 = 2
			// 6-7 / 2 = 3
			int dst_byte_idx = i / 2;
			dst_byte_idx += byte_offset;

			// dst_byte_idx must be adjusted too

			block[dst_byte_idx] |= ((block_copy[src_byte_idx] << i) & 0x1) >> bit_offset;
			bit_offset++;

		}

		bit_offset = 0;

	}



}
