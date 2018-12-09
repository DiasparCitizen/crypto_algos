
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "math.h"

uint8_t precomp_sbox[][16] = {
	{0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76},
	{0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47 ,0xf0, 0xad, 0xd4 ,0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0},
	{0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15},
	{0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75},
	{0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84},
	{0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf},
	{0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8},
	{0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2},
	{0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73},
	{0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb},
	{0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79},
	{0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08},
	{0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a},
	{0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e},
	{0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf},
	{0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16}
};

uint8_t rcon[11] = {0x8d, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36};

#define _S(b)	precomp_sbox[b >> 4][b & 0xF]

#define DEBUG_2
#define BLOCK_SIZE_BYTES	16

/*********** AES with 128 bit key FUNCTIONS ***********/

void print_array(uint8_t *text, int size){

	for (int i = 0; i < size; i++){
		printf("0x%x ", text[/*size-i-1*/i]);
	}
	printf("\n");

}

void print_array_pretty(uint8_t *text, int size){

	for (int i = 0; i < size; i++){
		printf("%02X ", text[/*size-i-1*/i]);
		if ( i > 0 && ((i+1) % BLOCK_SIZE_BYTES) == 0 )
			printf("\n");
	}
	printf("\n");

}

uint32_t _bswap32(uint32_t a){

  a = ((a & 0x000000FF) << 24) |
      ((a & 0x0000FF00) <<  8) |
      ((a & 0x00FF0000) >>  8) |
      ((a & 0xFF000000) >> 24);
  return a;

}

void sBox(uint8_t *block){

	block[0] = _S(block[0]);
	block[1] = _S(block[1]);
	block[2] = _S(block[2]);
	block[3] = _S(block[3]);

	block[4] = _S(block[4]);
	block[5] = _S(block[5]);
	block[6] = _S(block[6]);
	block[7] = _S(block[7]);

	block[8] = _S(block[8]);
	block[9] = _S(block[9]);
	block[10] = _S(block[10]);
	block[11] = _S(block[11]);

	block[12] = _S(block[12]);
	block[13] = _S(block[13]);
	block[14] = _S(block[14]);
	block[15] = _S(block[15]);

}

void shiftRows(uint8_t *block){

	uint8_t aux, aux2;

	// First row

	aux = block[1]; // B1
	block[1] = block[5]; // B1 <- B5

	aux2 = block[13]; // B13
	block[13] = aux; // B13 <- B1

	aux = block[9]; // B9
	block[9] = aux2; // B9 <- B13

	block[5] = aux; // B5 <- B9

	// Second row

	aux = block[2]; // B2
	block[2] = block[10]; // B2 <- B10

	block[10] = aux; // B10 <- B2

	aux = block[6]; // B6
	block[6] = block[14]; // B6 <- B14

	block[14] = aux; // B14 <- B6

	// Third row

	aux = block[3]; // B3
	block[3] = block[15]; // B3 <- B15

	aux2 = block[7]; // B7
	block[7] = aux; // B7 <- B3

	aux = block[11]; // B11
	block[11] = aux2; // B11 <- B7

	block[15] = aux; // B15 <- B11

}

uint8_t gmul(uint8_t a, uint8_t b) {


		uint8_t b_copy = b;
		uint8_t a_copy = a;


        uint8_t p = 0;
        uint8_t counter;
        uint8_t hi_bit_set;
        for (counter = 0; counter < 8; counter++) {
                if (b & 1)
                        p ^= a;
                hi_bit_set = (a & 0x80);
                a <<= 1;
                if (hi_bit_set)
                        a ^= 0x1b; // x^8 + x^4 + x^3 + x + 1
                b >>= 1;
        }

        return p;

}



void mixCol(uint8_t *block){

	uint8_t block_copy[16];

	for (int i = 0; i < 16; i++)
		block_copy[i] = block[i];

#define OFFSET 0

	block[0 + OFFSET] = gmul(0x02, block_copy[0 + OFFSET]) ^ gmul(0x03, block_copy[1 + OFFSET]) ^ block_copy[2 + OFFSET] ^ block_copy[3 + OFFSET];
	block[1 + OFFSET] = block_copy[0 + OFFSET] ^ gmul(0x02, block_copy[1 + OFFSET]) ^ gmul(0x03, block_copy[2 + OFFSET]) ^ block_copy[3 + OFFSET];
	block[2 + OFFSET] = block_copy[0 + OFFSET] ^ block_copy[1 + OFFSET] ^ gmul(0x02, block_copy[2 + OFFSET]) ^ gmul(0x03, block_copy[3 + OFFSET]);
	block[3 + OFFSET] = gmul(0x03, block_copy[0 + OFFSET]) ^ block_copy[1 + OFFSET] ^ block_copy[2 + OFFSET] ^ gmul(0x02, block_copy[3 + OFFSET]);

#undef OFFSET
#define OFFSET 4

	block[0 + OFFSET] = gmul(0x02, block_copy[0 + OFFSET]) ^ gmul(0x03, block_copy[1 + OFFSET]) ^ block_copy[2 + OFFSET] ^ block_copy[3 + OFFSET];
	block[1 + OFFSET] = block_copy[0 + OFFSET] ^ gmul(0x02, block_copy[1 + OFFSET]) ^ gmul(0x03, block_copy[2 + OFFSET]) ^ block_copy[3 + OFFSET];
	block[2 + OFFSET] = block_copy[0 + OFFSET] ^ block_copy[1 + OFFSET] ^ gmul(0x02, block_copy[2 + OFFSET]) ^ gmul(0x03, block_copy[3 + OFFSET]);
	block[3 + OFFSET] = gmul(0x03, block_copy[0 + OFFSET]) ^ block_copy[1 + OFFSET] ^ block_copy[2 + OFFSET] ^ gmul(0x02, block_copy[3 + OFFSET]);

#undef OFFSET
#define OFFSET 8

	block[0 + OFFSET] = gmul(0x02, block_copy[0 + OFFSET]) ^ gmul(0x03, block_copy[1 + OFFSET]) ^ block_copy[2 + OFFSET] ^ block_copy[3 + OFFSET];
	block[1 + OFFSET] = block_copy[0 + OFFSET] ^ gmul(0x02, block_copy[1 + OFFSET]) ^ gmul(0x03, block_copy[2 + OFFSET]) ^ block_copy[3 + OFFSET];
	block[2 + OFFSET] = block_copy[0 + OFFSET] ^ block_copy[1 + OFFSET] ^ gmul(0x02, block_copy[2 + OFFSET]) ^ gmul(0x03, block_copy[3 + OFFSET]);
	block[3 + OFFSET] = gmul(0x03, block_copy[0 + OFFSET]) ^ block_copy[1 + OFFSET] ^ block_copy[2 + OFFSET] ^ gmul(0x02, block_copy[3 + OFFSET]);

#undef OFFSET
#define OFFSET 12

	block[0 + OFFSET] = gmul(0x02, block_copy[0 + OFFSET]) ^ gmul(0x03, block_copy[1 + OFFSET]) ^ block_copy[2 + OFFSET] ^ block_copy[3 + OFFSET];
	block[1 + OFFSET] = block_copy[0 + OFFSET] ^ gmul(0x02, block_copy[1 + OFFSET]) ^ gmul(0x03, block_copy[2 + OFFSET]) ^ block_copy[3 + OFFSET];
	block[2 + OFFSET] = block_copy[0 + OFFSET] ^ block_copy[1 + OFFSET] ^ gmul(0x02, block_copy[2 + OFFSET]) ^ gmul(0x03, block_copy[3 + OFFSET]);
	block[3 + OFFSET] = gmul(0x03, block_copy[0 + OFFSET]) ^ block_copy[1 + OFFSET] ^ block_copy[2 + OFFSET] ^ gmul(0x02, block_copy[3 + OFFSET]);

#undef OFFSET

}

void doRound(uint8_t *block){

#ifdef DEBUG
	printf("********* NEW ROUND *********\n");
	print_array(block, 16);
#endif

	// Byte substitution layer
	sBox(block);
#ifdef DEBUG
	print_array(block, 16);
#endif

	// ShiftRows layer
	shiftRows(block);
#ifdef DEBUG
	print_array(block, 16);
#endif

	// MixColumn layer
	mixCol(block);
#ifdef DEBUG
	print_array(block, 16);
	printf("*****************************\n");
#endif

}

void doLastRound(uint8_t *block){

#ifdef DEBUG
	printf("********* LAST ROUND *********\n");
	print_array(block, 16);
#endif

	// Byte substitution layer
	sBox(block);
#ifdef DEBUG
	print_array(block, 16);
#endif
	// ShiftRows layer
	shiftRows(block);

#ifdef DEBUG
	print_array(block, 16);
	printf("******************************\n");
#endif

}

void g_function(uint8_t *g_result, uint8_t *prev_key, uint8_t round_num){

	g_result[0] = _S(prev_key[1]) ^ rcon[round_num];
	//g_result[0] = _S(g_result[0]);
	g_result[1] = _S(prev_key[2]);
	g_result[2] = _S(prev_key[3]);
	g_result[3] = _S(prev_key[0]);

}

void transform_key(uint8_t *key, uint8_t *next_key, uint8_t round_num){

#ifdef DEBUG
	printf("*********TRANSFORM KEY %d*************\n", round_num);
	printf("Old key: ");
	print_array(key, 16);
#endif

	uint8_t g_result[4];

	g_function(g_result, &key[12], round_num);

	next_key[0] = key[0] ^ g_result[0];
	next_key[1] = key[1] ^ g_result[1];
	next_key[2] = key[2] ^ g_result[2];
	next_key[3] = key[3] ^ g_result[3];

	next_key[4] = key[4] ^ next_key[0];
	next_key[5] = key[5] ^ next_key[1];
	next_key[6] = key[6] ^ next_key[2];
	next_key[7] = key[7] ^ next_key[3];

	next_key[8] = key[8] ^ next_key[4];
	next_key[9] = key[9] ^ next_key[5];
	next_key[10] = key[10] ^ next_key[6];
	next_key[11] = key[11] ^ next_key[7];

	next_key[12] = key[12] ^ next_key[8];
	next_key[13] = key[13] ^ next_key[9];
	next_key[14] = key[14] ^ next_key[10];
	next_key[15] = key[15] ^ next_key[11];

#ifdef DEBUG
	printf("New key: ");
	print_array(next_key, 16);
	printf("*************************************\n");
#endif

}

void add(uint8_t *block, uint8_t *block2){

	((uint32_t*)block)[0] ^= ((uint32_t*)block2)[0];
	((uint32_t*)block)[1] ^= ((uint32_t*)block2)[1];
	((uint32_t*)block)[2] ^= ((uint32_t*)block2)[2];
	((uint32_t*)block)[3] ^= ((uint32_t*)block2)[3];

}

void encrypt_block_AES(uint8_t *block, uint8_t *key){

	uint8_t aux_key[16];
	uint8_t aux_key_2[16];

	memcpy(aux_key_2, key, BLOCK_SIZE_BYTES);

	add(block, key);

	// Round 1
	doRound(block);

	transform_key(aux_key_2, aux_key, 1);
	add(block, aux_key);

	// Round 2
	doRound(block);

	transform_key(aux_key, aux_key_2, 2);
	add(block, aux_key_2);

	// Round 3
	doRound(block);

	transform_key(aux_key_2, aux_key, 3);
	add(block, aux_key);

	// Round 4
	doRound(block);

	transform_key(aux_key, aux_key_2, 4);
	add(block, aux_key_2);

	// Round 5
	doRound(block);

	transform_key(aux_key_2, aux_key, 5);
	add(block, aux_key);

	// Round 6
	doRound(block);

	transform_key(aux_key, aux_key_2, 6);
	add(block, aux_key_2);

	// Round 7
	doRound(block);

	transform_key(aux_key_2, aux_key, 7);
	add(block, aux_key);

	// Round 8
	doRound(block);

	transform_key(aux_key, aux_key_2, 8);
	add(block, aux_key_2);

	// Round 9
	doRound(block);

	transform_key(aux_key_2, aux_key, 9);
	add(block, aux_key);

	// Round 10
	doLastRound(block);

	transform_key(aux_key, aux_key_2, 10);
	add(block, aux_key_2);

}

/*********** GALOIS COUNTER MODE (GCM) FUNCTIONS ***********/

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
	printf("\ncounter 0:\n");
	print_array_pretty(counter, 16);

	// Encrypt counter 0 for later
	memcpy(counter0_copy, counter, 16);
	encrypt_block_AES(counter0_copy, key);

	// Initialize H
	// The initial value of H is the result
	// of encrypting the all-zero input with the block
	// cipher
	encrypt_block_AES(H, key);
	printf("\nH:\n");
	print_array_pretty(H, 16);

	// g0 is the result of the multiplication in GF(2^128)
	// of H and AAD
	galois_128_mult_lle(H, aad, g_sub_i);

	// Initialize len(A)||len(C)
	uint8_t lenA_lenC[BLOCK_SIZE_BYTES] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00};





	// BEGIN ENCRYPTION

	for (int b_index = 0; b_index < data_size; b_index += BLOCK_SIZE_BYTES){

		curr_index = b_index;

		increase_counter(counter);

#ifdef DEBUG_2
	printf("New counter: ");
	print_array(counter, BLOCK_SIZE_BYTES);
#endif

	    memcpy(encryption_input, counter, BLOCK_SIZE_BYTES);

	    // E(encryption_input, K)
		encrypt_block_AES(encryption_input, key);

#ifdef DEBUG_2
	printf("After AES: ");
	print_array(encryption_input, BLOCK_SIZE_BYTES);
#endif

		// Yi = Xi + E(encryption_input, K)
		add(&(data[b_index]), encryption_input);

#ifdef DEBUG_2
	printf("Add: ");
	print_array(&(data[b_index]), BLOCK_SIZE_BYTES);
#endif

		// g_sub_i = Yi + g_sub_i
		add(g_sub_i, &(data[b_index]));
		galois_128_mult_lle(H, g_sub_i, aux);
		memcpy(g_sub_i, aux, 16);

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

int main(void) {

	//http://testprotect.com/appendix/AEScalc

	puts("BEGIN!");

	uint8_t key[16] = {0xfe, 0xff, 0xe9, 0x92, 0x86, 0x65, 0x73, 0x1c, 0x6d, 0x6a, 0x8f, 0x94, 0x67, 0x30, 0x83, 0x08};
	uint8_t plaintext[64] = {0xd9, 0x31, 0x32, 0x25, 0xf8, 0x84, 0x06, 0xe5, 0xa5, 0x59, 0x09, 0xc5, 0xaf, 0xf5, 0x26, 0x9a,
			0x86, 0xa7, 0xa9, 0x53, 0x15, 0x34, 0xf7, 0xda, 0x2e, 0x4c, 0x30, 0x3d, 0x8a, 0x31, 0x8a, 0x72,
			0x1c, 0x3c, 0x0c, 0x95, 0x95, 0x68, 0x09, 0x53, 0x2f, 0xcf, 0x0e, 0x24, 0x49, 0xa6, 0xb5, 0x25,
			0xb1, 0x6a, 0xed, 0xf5, 0xaa, 0x0d, 0xe6, 0x57, 0xba, 0x63, 0x7b, 0x39, 0x1a, 0xaf, 0xd2, 0x55};
	uint8_t iv[12] = {0xca, 0xfe, 0xba, 0xbe, 0xfa, 0xce, 0xdb, 0xad, 0xde, 0xca, 0xf8, 0x88};
	//feed face dead beef feed face dead beef abad dad2
	uint8_t aad[20] = {0xfe, 0xed, 0xfa, 0xce, 0xde, 0xad, 0xbe, 0xef, 0xfe, 0xed, 0xfa, 0xce, 0xde, 0xad, 0xbe, 0xef, 0xab, 0xad, 0xda, 0xd2};

	encrypt_data_AES_GCM(plaintext, 64, key, iv, aad);

	printf("\nCipher text:\n");
	print_array_pretty(plaintext, 64);

	puts("END!");



	/*uint8_t region_x[16];
	uint8_t region_y[16];
	uint8_t region_z[16];
	memset(region_x, 0x0, 16);
	memset(region_y, 0x0, 16);
	memset(region_z, 0x0, 16);

	region_x[0] = 0b10100000;
	region_y[0] = 0b11100000;

	print_region(region_x);
	print_region(region_y);
	print_region(region_z);

	printf("GALOIS!\n");
	galois_128_mult_lle(region_x, region_y, region_z);
	print_region(region_z);*/

	return EXIT_SUCCESS;

}
