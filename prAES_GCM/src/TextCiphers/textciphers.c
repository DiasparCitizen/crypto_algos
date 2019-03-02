/*
 * textciphers.c
 *
 *  Created on: 27 feb. 2019
 *      Author: Ich
 */

#include "textciphers.h"



void increase_counter(uint8_t *counter){

	uint32_t copy = _bswap32( ((uint32_t*)counter)[3] ) + 1;
	((uint32_t*)counter)[3] = _bswap32( copy );

#ifdef DEBUG
	print_array(counter, 16);
#endif

}


void encrypt_data_AES_GCM(uint8_t *data, int data_size, uint8_t key[BLOCK_SIZE_BYTES], uint8_t iv[12], uint8_t *aad){


	// Declare vars
	int curr_index;
	uint8_t encryption_input[BLOCK_SIZE_BYTES];
	uint8_t counter[BLOCK_SIZE_BYTES] = {0x0};
	uint8_t H[BLOCK_SIZE_BYTES] = {0x0};
	uint8_t g_sub_i[BLOCK_SIZE_BYTES] = {0x0};
	uint8_t aux[BLOCK_SIZE_BYTES] = {0x0};
	uint8_t counter0_copy[BLOCK_SIZE_BYTES] = {0x0};



	// Initialize the counter
	memcpy(counter, iv, 12);
	increase_counter(counter); // Y0 == IV || 0x00000001
	printf("counter 0:\n");
	print_array_pretty(counter, 16);

	// Encrypt counter 0 for later
	memcpy(counter0_copy, counter, 16);
	encrypt_block_AES(counter0_copy, key);

	// Initialize H
	// The initial value of H is the result
	// of encrypting the all-zero input with the block
	// cipher
	encrypt_block_AES(H, key);
	printf("H:\n");
	print_array_pretty(H, 16);

	// g0 is the result of the multiplication in GF(2^128)
	// of H and AAD
	galois_128_mult_lle(aad, H, g_sub_i);
	printf("g sub 0:\n");
	print_array_pretty(g_sub_i, 16);

	// Initialize len(A)||len(C)
	uint8_t lenA_lenC[BLOCK_SIZE_BYTES] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00};





	// BEGIN ENCRYPTION

	for (int b_index = 0; b_index < data_size; b_index += BLOCK_SIZE_BYTES){

		curr_index = b_index;

		// Increase the counter
		increase_counter(counter);

		//printf("New counter: ");
		//print_array(counter, BLOCK_SIZE_BYTES);

	    memcpy(encryption_input, counter, BLOCK_SIZE_BYTES);

	    // E(encryption_input, K)
		encrypt_block_AES(encryption_input, key);

		//printf("After AES: ");
		//print_array(encryption_input, BLOCK_SIZE_BYTES);

		// Yi = Xi + E(encryption_input, K)
		add(&(data[b_index]), encryption_input);

		//printf("Add: ");
		//print_array(&(data[b_index]), BLOCK_SIZE_BYTES);

		// g_sub_i = Yi + g_sub_i
		add(g_sub_i, &(data[b_index]));
		galois_128_mult_lle(g_sub_i, H, aux);
		memcpy(g_sub_i, aux, 16);
		printf("g sub %d:\n", ((b_index/BLOCK_SIZE_BYTES)+1));
		print_array_pretty(g_sub_i, 16);

	}

	// g_sub_i Xor lenA_lenC
	add(g_sub_i, lenA_lenC);

	// Multiply again by H
	galois_128_mult_lle(H, g_sub_i, aux);

	// Calculate hash
	add(aux, counter0_copy);
	printf("hash: ");
	print_array(aux, BLOCK_SIZE_BYTES);

}
