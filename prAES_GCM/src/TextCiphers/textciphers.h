/*
 * textciphers.h
 *
 *  Created on: 27 feb. 2019
 *      Author: Ich
 */

#ifndef TEXTCIPHERS_TEXTCIPHERS_H_
#define TEXTCIPHERS_TEXTCIPHERS_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "../BlockCiphers/AES.h"
#include "../Helper/helper.h"

// Galois counter mode
void encrypt_data_AES_GCM(uint8_t *data,
						uint16_t data_byte_len,
						uint8_t key[BLOCK_SIZE_BYTES],
						uint8_t iv[12],
						uint8_t *aad,
						uint16_t aad_byte_len,
						uint8_t *tag);

// Electronic code book
void encrypt_ECB(uint8_t *input_data,
				uint32_t data_byte_len,
				uint8_t *output_data,
				uint8_t *key,
				uint16_t key_byte_len,
				void (*block_cipher)(),
				uint16_t block_byte_len);

// Cipher block chaining
void encrypt_CBC(uint8_t *input_data,
				uint32_t data_byte_len,
				uint8_t *output_data,
				uint8_t *key,
				uint16_t key_byte_len,
				uint8_t *iv,
				uint16_t iv_byte_len,
				void (*block_cipher)(),
				uint16_t block_byte_len);

// Output feedback
void encrypt_OFB(uint8_t *input_data,
				uint32_t data_byte_len,
				uint8_t *output_data,
				uint8_t *key,
				uint16_t key_byte_len,
				uint8_t *iv,
				uint16_t iv_byte_len,
				void (*block_cipher)(),
				uint16_t block_byte_len);

// Counter Mode Encryption (CTR)
void encrypt_CTR(uint8_t *input_data,
				uint32_t data_byte_len,
				uint8_t *output_data,
				uint8_t *key,
				uint16_t key_byte_len,
				uint8_t *iv,
				uint16_t iv_byte_len,
				uint16_t ctr_byte_len,
				void (*block_cipher)(),
				uint16_t block_byte_len);

#endif /* TEXTCIPHERS_TEXTCIPHERS_H_ */
