
#ifndef BLOCKCIPHERS_DES_H_
#define BLOCKCIPHERS_DES_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "../Math/math.h"
#include "../Helper/helper.h"

//#define DES_FFUNCTION_DEBUG
//#define DES_BLOCK_ENCRYPT_DEBUG

#define DES_BLOCK_BIT_LEN 64
#define DES_BLOCK_BYTE_LEN (DES_BLOCK_BIT_LEN / 8)

#define DES_KEY_BIT_LEN 56
#define DES_KEY_BYTE_LEN (DES_KEY_BIT_LEN / 8)

#define DES_EXPANDED_HALF_BLOCK_BIT_LEN 48
#define DES_EXPANDED_HALF_BLOCK_BYTE_LEN (DES_EXPANDED_HALF_BLOCK_BIT_LEN / 8)

#define NUM_ROUNDS 16

#define _m_flip_bit(bit) bit = 7 - bit

#define RIGHT(blck) (&(blck[4]))
#define BLOCK64(blck) (((uint64_t*)blck)[0])
#define BLOCK32(blck) (((uint32_t*)blck)[0])

/* Prototypes */

// Simple DES block cipher
void des_block_encrypt(uint8_t * block, uint8_t * key);
void des_block_decrypt(uint8_t * block, uint8_t * key);

// 3DES with double length key
void des_block_encrypt_3DES_2TDEA(uint8_t * block, uint8_t * key);
void des_block_decrypt_3DES_2TDEA(uint8_t * block, uint8_t * key);

// 3DES with triple length key
void des_block_encrypt_3DES_3TDEA(uint8_t * block, uint8_t * key);
void des_block_decrypt_3DES_3TDEA(uint8_t * block, uint8_t * key);

#endif /* BLOCKCIPHERS_DES_H_ */
