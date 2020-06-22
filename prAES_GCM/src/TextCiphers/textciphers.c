
#include "textciphers.h"

void increase_counter(uint8_t *counter){

	uint32_t copy = _bswap32( ((uint32_t*)counter)[3] ) + 1;
	((uint32_t*)counter)[3] = _bswap32( copy );

}


void encrypt_data_AES_GCM(uint8_t *data,
						uint16_t data_byte_len,
						uint8_t key[BLOCK_SIZE_BYTES],
						uint8_t iv[12],
						uint8_t *aad,
						uint16_t aad_byte_len,
						uint8_t *tag){

	// Declare vars
	uint8_t encryption_input[BLOCK_SIZE_BYTES];
	uint8_t counter[BLOCK_SIZE_BYTES] = {0x0};
	uint8_t H[BLOCK_SIZE_BYTES] = {0x0};
	uint8_t g_sub_i[BLOCK_SIZE_BYTES] = {0x0};
	uint8_t aux[BLOCK_SIZE_BYTES] = {0x0};
	uint8_t counter0_copy[BLOCK_SIZE_BYTES] = {0x0};

	// Initialize the counter
	memcpy(counter, iv, 12);
	increase_counter(counter); // Y0 == IV || 0x00000001

	// Encrypt counter 0 for later
	memcpy(counter0_copy, counter, 16);
	encrypt_block_AES(counter0_copy, key);

	// Initialize H
	// The initial value of H is the result
	// of encrypting the all-zero input with the block
	// cipher
	encrypt_block_AES(H, key);
	//printf("H:\n");
	//print_array_pretty(H, 16);

	// g0 is the result of the multiplication in GF(2^128)
	// of H and AAD
	galois_128_mult(aad, H, g_sub_i);
	//printf("g sub 0:\n");
	//print_array_pretty(g_sub_i, 16);

	// Initialize len(A)||len(C). Lengths are expressed in bits!
	uint8_t lenA_lenC[BLOCK_SIZE_BYTES];
	((uint64_t*)lenA_lenC)[0] = _bswap64((uint64_t)aad_byte_len * 8);
	((uint64_t*)lenA_lenC)[1] = _bswap64((uint64_t)data_byte_len * 8);

	// BEGIN ENCRYPTION

	for (int b_index = 0; b_index < data_byte_len; b_index += BLOCK_SIZE_BYTES){

		//printf("***********************\nEncrypt block #%d\n", (b_index/BLOCK_SIZE_BYTES));

		// Increase the counter
		increase_counter(counter);

		//printf("New counter: ");
		//print_array(counter, BLOCK_SIZE_BYTES);

	    memcpy(encryption_input, counter, BLOCK_SIZE_BYTES);

	    // E(encryption_input, K)
		encrypt_block_AES(encryption_input, key);

		//printf("E(K, Y%d)\n", ((b_index/16)+1));
		//print_array_pretty(encryption_input, BLOCK_SIZE_BYTES);

		// Yi = Xi xor E(encryption_input, K)
		xor(&(data[b_index]), encryption_input);

		//printf("Add: ");
		//print_array(&(data[b_index]), BLOCK_SIZE_BYTES);

		// g_sub_i = Yi + g_sub_i
		xor(g_sub_i, &(data[b_index]));
		galois_128_mult(g_sub_i, H, aux);
		memcpy(g_sub_i, aux, BLOCK_SIZE_BYTES);
		//printf("g sub %d:\n", ((b_index/BLOCK_SIZE_BYTES)+1));
		//print_array_pretty(g_sub_i, 16);

	}

	// g_sub_i Xor lenA_lenC
	xor(g_sub_i, lenA_lenC);

	// Multiply again by H
	galois_128_mult(g_sub_i, H, aux);
	// Calculate hash
	xor(aux, counter0_copy);

	memcpy(tag, aux, BLOCK_SIZE_BYTES);

}

void encrypt_ECB(uint8_t *input_data,
				uint32_t data_byte_len,
				uint8_t *output_data,
				uint8_t *key,
				uint16_t key_byte_len,
				void (*block_cipher)(),
				uint16_t block_byte_len){

	// It's not ideal, but we can do this temporarily
	memcpy(output_data, input_data, data_byte_len);

	for (uint32_t b_index = 0; b_index < data_byte_len; b_index += block_byte_len){
		block_cipher(&(output_data[b_index]), key);
	}

}

void encrypt_CBC(uint8_t *input_data,
				uint32_t data_byte_len,
				uint8_t *output_data,
				uint8_t *key,
				uint16_t key_byte_len,
				uint8_t *iv,
				uint16_t iv_byte_len,
				void (*block_cipher)(),
				uint16_t block_byte_len){


	// It's not ideal, but we can do this temporarily
	memcpy(output_data, input_data, data_byte_len);

	for (uint32_t b_index = 0; b_index < data_byte_len; b_index += block_byte_len){

		if (b_index == 0){
			// First it: xor with IV
			// IV should be the size of the block
			xor_bytes(output_data, iv, block_byte_len);
		}else{
			//            Still plaintext    XOR        encrypted block in last iter
			xor_bytes(&(output_data[b_index]), &(output_data[b_index - block_byte_len]), block_byte_len);
		}

		block_cipher(&(output_data[b_index]), key);

	}

}

void encrypt_CTR(uint8_t *input_data,
				uint32_t data_byte_len,
				uint8_t *output_data,
				uint8_t *key,
				uint16_t key_byte_len,
				uint8_t *iv,
				uint16_t iv_byte_len,
				uint16_t ctr_byte_len,
				void (*block_cipher)(),
				uint16_t block_byte_len){

	// It's not ideal, but we can do this temporarily
	memcpy(output_data, input_data, data_byte_len);

	uint8_t ctr[block_byte_len];
	uint8_t input[block_byte_len];
	memset(ctr, 0x0, block_byte_len);

	// Calculate some params
	uint32_t max_ctr_value = ipow(2, ctr_byte_len*8);
	uint32_t counter = 0x0;
	uint32_t counter_cpy;

	// Copy nonce
	memcpy(ctr, iv, iv_byte_len);

	// Initialize counter
	memcpy(&counter, (ctr + iv_byte_len - ctr_byte_len), ctr_byte_len);
	reverseBytes(&counter, ctr_byte_len);
	//printf("counter: %04x\n", counter);
	//print_array_pretty(ctr, 16);


	for (uint32_t b_index = 0; b_index < data_byte_len; b_index += block_byte_len){

		memcpy(input, ctr, block_byte_len);
		//printf("input:\n");
		//print_array_pretty(ctr, 16);

		block_cipher(input, key);
		//printf("output:\n");
		//print_array_pretty(input, 16);

		xor_bytes(&(output_data[b_index]), input, block_byte_len);

		counter = (counter + 1) % max_ctr_value;
		counter_cpy = counter;
		reverseBytes(&counter_cpy, ctr_byte_len);
		memcpy((ctr + iv_byte_len - ctr_byte_len), &counter_cpy, ctr_byte_len);


	}

}

void encrypt_OFB(uint8_t *input_data,
				uint32_t data_byte_len,
				uint8_t *output_data,
				uint8_t *key,
				uint16_t key_byte_len,
				uint8_t *iv,
				uint16_t iv_byte_len,
				void (*block_cipher)(),
				uint16_t block_byte_len){

	// It's not ideal, but we can do this temporarily
	memcpy(output_data, input_data, data_byte_len);

	uint8_t aux[block_byte_len];
	memcpy(aux, iv, block_byte_len);

	for (uint32_t b_index = 0; b_index < data_byte_len; b_index += block_byte_len){

		if (b_index == 0){
			// Encrypt IV
			block_cipher(aux, key);
			xor_bytes(&(output_data[b_index]), aux, block_byte_len);
		}else{
			block_cipher(aux, key);
			xor_bytes(&(output_data[b_index]), aux, block_byte_len);
		}

	}

}

