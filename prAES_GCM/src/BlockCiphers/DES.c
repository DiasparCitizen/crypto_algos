
#include "DES.h"

/* Prototypes */

//void permute_overwrite_and_flip(uint8_t * block, const int ** bit_map, uint8_t dst_len);
//void permute_and_flip(uint8_t * dst_block, uint8_t * src_block, const int ** bit_map, uint8_t dst_len);
void xor_48(uint8_t * a, uint8_t * b);
void xor_32(uint8_t * a, uint8_t * b);
void rightLeftSwap(uint8_t * block);
void f_function(uint8_t * half_block, uint8_t * key);
void des_round(uint8_t * block, uint8_t * key);

// Key schedule tables

const int PC1[56] = {
   57, 49, 41, 33, 25, 17,  9,
    1, 58, 50, 42, 34, 26, 18,
   10,  2, 59, 51, 43, 35, 27,
   19, 11,  3, 60, 52, 44, 36,
   63, 55, 47, 39, 31, 23, 15,
    7, 62, 54, 46, 38, 30, 22,
   14,  6, 61, 53, 45, 37, 29,
   21, 13,  5, 28, 20, 12,  4
};
const int Rotations[16] = {
    1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1
};
const int PC2[48] = {
   14, 17, 11, 24,  1,  5,
    3, 28, 15,  6, 21, 10,
   23, 19, 12,  4, 26,  8,
   16,  7, 27, 20, 13,  2,
   41, 52, 31, 37, 47, 55,
   30, 40, 51, 45, 33, 48,
   44, 49, 39, 56, 34, 53,
   46, 42, 50, 36, 29, 32
};
const int RotateLeft[2][56] = {
	{
	   2, 3, 4, 5, 6, 7, 8, 9,
	   10, 11, 12, 13, 14, 15, 16, 17,
	   18, 19, 20, 21, 22, 23, 24, 25,
	   26, 27, 28, 1,
	   30, 31, 32, 33, 34, 35, 36, 37,
	   38, 39, 40, 41, 42, 43, 44, 45,
	   46, 47, 48, 49, 50, 51, 52, 53,
	   54, 55, 56, 29
	},
	{
	   3, 4, 5, 6, 7, 8, 9, 10,
	   11, 12, 13, 14, 15, 16, 17, 18,
	   19, 20, 21, 22, 23, 24, 25, 26,
	   27, 28, 1, 2,
	   31, 32, 33, 34, 35, 36, 37, 38,
	   39, 40, 41, 42, 43, 44, 45, 46,
	   47, 48, 49, 50, 51, 52, 53, 54,
	   55, 56, 29, 30
	}
};

const int RotateRight[2][56] = {
	{
	   28, 1, 2, 3, 4, 5, 6, 7,
	   8, 9, 10, 11, 12, 13, 14, 15,
	   16, 17, 18, 19, 20, 21, 22, 23,
	   24, 25, 26, 27,
	   56, 29, 30, 31, 32, 33, 34, 35,
	   36, 37, 38, 39, 40, 41, 42, 43,
	   44, 45, 46, 47, 48, 49, 50, 51,
	   52, 53, 54, 55
	},
	{
	   27, 28, 1, 2, 3, 4, 5, 6,
	   7, 8, 9, 10, 11, 12, 13, 14,
	   15, 16, 17, 18, 19, 20, 21, 22,
	   23, 24, 25, 26,
	   55, 56, 29, 30, 31, 32, 33, 34,
	   35, 36, 37, 38, 39, 40, 41, 42,
	   43, 44, 45, 46, 47, 48, 49, 50,
	   51, 52, 53, 54
	}
};

// Permutation tables

const int InitialPermutation[64] = {
    58, 50, 42, 34, 26, 18, 10,  2,
    60, 52, 44, 36, 28, 20, 12,  4,
    62, 54, 46, 38, 30, 22, 14,  6,
    64, 56, 48, 40, 32, 24, 16,  8,
    57, 49, 41, 33, 25, 17,  9,  1,
    59, 51, 43, 35, 27, 19, 11,  3,
    61, 53, 45, 37, 29, 21, 13,  5,
    63, 55, 47, 39, 31, 23, 15,  7
};
const int FinalPermutation[64] = {
    40,  8, 48, 16, 56, 24, 64, 32,
    39,  7, 47, 15, 55, 23, 63, 31,
    38,  6, 46, 14, 54, 22, 62, 30,
    37,  5, 45, 13, 53, 21, 61, 29,
    36,  4, 44, 12, 52, 20, 60, 28,
    35,  3, 43, 11, 51, 19, 59, 27,
    34,  2, 42, 10, 50, 18, 58, 26,
    33,  1, 41,  9, 49, 17, 57, 25
};

// Rounds tables

const int DesExpansion[48] = {
    32,  1,  2,  3,  4,  5,  4,  5,
     6,  7,  8,  9,  8,  9, 10, 11,
    12, 13, 12, 13, 14, 15, 16, 17,
    16, 17, 18, 19, 20, 21, 20, 21,
    22, 23, 24, 25, 24, 25, 26, 27,
    28, 29, 28, 29, 30, 31, 32,  1
};

const int Pbox[32] = {
    16,  7, 20, 21, 29, 12, 28, 17,
     1, 15, 23, 26,  5, 18, 31, 10,
     2,  8, 24, 14, 32, 27,  3,  9,
    19, 13, 30,  6, 22, 11,  4, 25
};

// S-Boxes

static const int SBox[8][64] = {
    {  /* S0 */
    14,  0,  4, 15, 13,  7,  1,  4,  2, 14, 15,  2, 11, 13,  8,  1,
     3, 10, 10,  6,  6, 12, 12, 11,  5,  9,  9,  5,  0,  3,  7,  8,
     4, 15,  1, 12, 14,  8,  8,  2, 13,  4,  6,  9,  2,  1, 11,  7,
    15,  5, 12, 11,  9,  3,  7, 14,  3, 10, 10,  0,  5,  6,  0, 13
    },
    {  /* S1 */
    15,  3,  1, 13,  8,  4, 14,  7,  6, 15, 11,  2,  3,  8,  4, 14,
     9, 12,  7,  0,  2,  1, 13, 10, 12,  6,  0,  9,  5, 11, 10,  5,
     0, 13, 14,  8,  7, 10, 11,  1, 10,  3,  4, 15, 13,  4,  1,  2,
     5, 11,  8,  6, 12,  7,  6, 12,  9,  0,  3,  5,  2, 14, 15,  9
    },
    {  /* S2 */
    10, 13,  0,  7,  9,  0, 14,  9,  6,  3,  3,  4, 15,  6,  5, 10,
     1,  2, 13,  8, 12,  5,  7, 14, 11, 12,  4, 11,  2, 15,  8,  1,
    13,  1,  6, 10,  4, 13,  9,  0,  8,  6, 15,  9,  3,  8,  0,  7,
    11,  4,  1, 15,  2, 14, 12,  3,  5, 11, 10,  5, 14,  2,  7, 12
    },
    {  /* S3 */
     7, 13, 13,  8, 14, 11,  3,  5,  0,  6,  6, 15,  9,  0, 10,  3,
     1,  4,  2,  7,  8,  2,  5, 12, 11,  1, 12, 10,  4, 14, 15,  9,
    10,  3,  6, 15,  9,  0,  0,  6, 12, 10, 11,  1,  7, 13, 13,  8,
    15,  9,  1,  4,  3,  5, 14, 11,  5, 12,  2,  7,  8,  2,  4, 14
    },
    {  /* S4 */
     2, 14, 12, 11,  4,  2,  1, 12,  7,  4, 10,  7, 11, 13,  6,  1,
     8,  5,  5,  0,  3, 15, 15, 10, 13,  3,  0,  9, 14,  8,  9,  6,
     4, 11,  2,  8,  1, 12, 11,  7, 10,  1, 13, 14,  7,  2,  8, 13,
    15,  6,  9, 15, 12,  0,  5,  9,  6, 10,  3,  4,  0,  5, 14,  3
    },
    {  /* S5 */
    12, 10,  1, 15, 10,  4, 15,  2,  9,  7,  2, 12,  6,  9,  8,  5,
     0,  6, 13,  1,  3, 13,  4, 14, 14,  0,  7, 11,  5,  3, 11,  8,
     9,  4, 14,  3, 15,  2,  5, 12,  2,  9,  8,  5, 12, 15,  3, 10,
     7, 11,  0, 14,  4,  1, 10,  7,  1,  6, 13,  0, 11,  8,  6, 13
    },
    {  /* S6 */
     4, 13, 11,  0,  2, 11, 14,  7, 15,  4,  0,  9,  8,  1, 13, 10,
     3, 14, 12,  3,  9,  5,  7, 12,  5,  2, 10, 15,  6,  8,  1,  6,
     1,  6,  4, 11, 11, 13, 13,  8, 12,  1,  3,  4,  7, 10, 14,  7,
    10,  9, 15,  5,  6,  0,  8, 15,  0, 14,  5,  2,  9,  3,  2, 12
    },
    {  /* S7 */
    13,  1,  2, 15,  8, 13,  4,  8,  6, 10, 15,  3, 11,  7,  1,  4,
    10, 12,  9,  5,  3,  6, 14, 11,  5,  0,  0, 14, 12,  9,  7,  2,
     7,  2, 11,  1,  4, 14,  1,  7,  9,  4, 12, 10, 14,  8,  2, 13,
     0, 15,  6, 12, 10,  9, 13,  0, 15,  3,  3,  5,  5,  6,  8, 11
    }
};

void permute_overwrite_and_flip(uint8_t * block, const int ** bit_map, uint8_t dst_len){

	// Make a copy of the block
	uint8_t block_copy[dst_len];
	memcpy(&block_copy, block, dst_len);

	// Clean block
	memset(block, 0, sizeof block_copy);

	for (uint8_t dst_bit = 0; dst_bit < 64; dst_bit++){

		uint8_t src_bit = (int)bit_map[dst_bit] - 1;

		uint8_t src_byte_bit = src_bit % 8;
		uint8_t src_byte = src_bit / 8;
		_m_flip_bit(src_byte_bit);

		uint8_t dst_byte_bit = dst_bit % 8;
		uint8_t dst_byte = dst_bit / 8;
		_m_flip_bit(dst_byte_bit);

		block[dst_byte] |= ((block_copy[src_byte] >> src_byte_bit) & 0x1) << dst_byte_bit;

	}

}

void permute_and_flip(uint8_t * dst_block, uint8_t * src_block, const int ** bit_map, uint8_t dst_len){

	// Clean block
	memset(dst_block, 0, dst_len);

	for (uint8_t dst_bit = 0; dst_bit < 64; dst_bit++){

		uint8_t src_bit = (int)bit_map[dst_bit] - 1;

		uint8_t src_byte_bit = src_bit % 8;
		uint8_t src_byte = src_bit / 8;
		_m_flip_bit(src_byte_bit);

		uint8_t dst_byte_bit = dst_bit % 8;
		uint8_t dst_byte = dst_bit / 8;
		_m_flip_bit(dst_byte_bit);

		dst_block[dst_byte] |= ((src_block[src_byte] >> src_byte_bit) & 0x1) << dst_byte_bit;

	}

}

void xor_48(uint8_t * a, uint8_t * b){
	((uint32_t*)a)[0] ^= ((uint32_t*)b)[0];
	((uint16_t*)a)[2] ^= ((uint16_t*)b)[2];
}

void xor_32(uint8_t * a, uint8_t * b){
	BLOCK32(a) ^= BLOCK32(b);
}

void rightLeftSwap(uint8_t * block){
	uint64_t aux = BLOCK64(block);
	BLOCK64(block) >>= 32;
	BLOCK64(block) |= aux << 32;
}

void des_round(uint8_t * block, uint8_t * key){

	uint8_t temp_block[DES_BLOCK_BYTE_LEN] = {0x0};
	BLOCK64(temp_block) = BLOCK64(block);

	// L[n+1] = R[n], R[n+1] = L[n]
	BLOCK64(block) >>= (DES_BLOCK_BIT_LEN/2);
	BLOCK64(block) |= BLOCK64(temp_block) << (DES_BLOCK_BIT_LEN/2);

	// R'[n] = f(R[n])
	f_function( RIGHT(temp_block), key );

	//R[n+1] = R'[n] xOR R[n]
	xor_32( RIGHT(block), RIGHT(temp_block) );

}

void f_function(uint8_t * half_block, uint8_t * key){

#ifdef DES_FFUNCTION_DEBUG
	printf("    f_function()\n");
	print_array_pretty(half_block, 4);
#endif

	uint8_t expanded_half_block[DES_EXPANDED_HALF_BLOCK_BYTE_LEN] = {0x00};

	// (1) EXPAND STEP
	//expand(expanded_block, half_block);
	//flip_bytes(expanded_block, 6);
	permute_and_flip(expanded_half_block, half_block, (const int **)&DesExpansion, DES_EXPANDED_HALF_BLOCK_BYTE_LEN);
#ifdef DES_FFUNCTION_DEBUG
	printf("    (1) EXPAND STEP\n");
	print_array_pretty(expanded_half_block, 6);
#endif

	// (2) XOR KEY STEP
	xor_48(expanded_half_block, key);
#ifdef DES_FFUNCTION_DEBUG
	printf("    (2) XOR KEY STEP\n");
	print_array_pretty(expanded_half_block, 6);
#endif
	// (3) SBOX STEP

	uint8_t sbox_output;
	uint8_t sbox_input;

	// Clear half_block
	memset(half_block, 0, 4);

#define SBOX_BASE 0
#define HALF_BLOCK_BASE 0
#define EXPANDED_BLOCK_BASE 0

	sbox_input = expanded_half_block[EXPANDED_BLOCK_BASE] >> 2;
	sbox_output = SBox[SBOX_BASE][sbox_input];
	half_block[HALF_BLOCK_BASE] |= sbox_output << 4;

	sbox_input = expanded_half_block[EXPANDED_BLOCK_BASE + 1] >> 4;
	sbox_input |= (expanded_half_block[EXPANDED_BLOCK_BASE] & OOOOOOII) << 4;
	sbox_output = SBox[SBOX_BASE + 1][sbox_input];
	half_block[HALF_BLOCK_BASE] |= sbox_output;

	sbox_input = expanded_half_block[EXPANDED_BLOCK_BASE + 2] >> 6;
	sbox_input |= (expanded_half_block[EXPANDED_BLOCK_BASE + 1] & OOOOIIII) << 2;
	sbox_output = SBox[SBOX_BASE + 2][sbox_input];
	half_block[HALF_BLOCK_BASE + 1] |= sbox_output << 4;

	sbox_input = expanded_half_block[EXPANDED_BLOCK_BASE + 2] & OOIIIIII;
	sbox_output = SBox[SBOX_BASE + 3][sbox_input];
	half_block[HALF_BLOCK_BASE + 1] |= sbox_output;

#undef SBOX_BASE
#undef HALF_BLOCK_BASE
#undef EXPANDED_BLOCK_BASE

#define SBOX_BASE 4
#define HALF_BLOCK_BASE 2
#define EXPANDED_BLOCK_BASE 3

	sbox_input = expanded_half_block[EXPANDED_BLOCK_BASE] >> 2;
	sbox_output = SBox[SBOX_BASE][sbox_input];
	half_block[HALF_BLOCK_BASE] |= sbox_output << 4;

	sbox_input = expanded_half_block[EXPANDED_BLOCK_BASE + 1] >> 4;
	sbox_input |= (expanded_half_block[EXPANDED_BLOCK_BASE] & OOOOOOII) << 4;
	sbox_output = SBox[SBOX_BASE + 1][sbox_input];
	half_block[HALF_BLOCK_BASE] |= sbox_output;

	sbox_input = expanded_half_block[EXPANDED_BLOCK_BASE + 2] >> 6;
	sbox_input |= (expanded_half_block[EXPANDED_BLOCK_BASE + 1] & OOOOIIII) << 2;
	sbox_output = SBox[SBOX_BASE + 2][sbox_input];
	half_block[HALF_BLOCK_BASE + 1] |= sbox_output << 4;

	sbox_input = expanded_half_block[EXPANDED_BLOCK_BASE + 2] & OOIIIIII;
	sbox_output = SBox[SBOX_BASE + 3][sbox_input];
	half_block[HALF_BLOCK_BASE + 1] |= sbox_output;

#undef SBOX_BASE
#undef HALF_BLOCK_BASE
#undef EXPANDED_BLOCK_BASE

#ifdef DES_FFUNCTION_DEBUG
	printf("    (3) SBOX STEP\n");
	print_array_pretty(half_block, 4);
#endif

	// (4) Permutation P step
	permute_overwrite_and_flip(half_block, (const int **)&Pbox, (DES_BLOCK_BYTE_LEN/2));
#ifdef DES_FFUNCTION_DEBUG
	printf("    (4) Permutation P step\n");
	print_array_pretty(half_block, 4);
#endif

}

/* Triple length key: k1, k2, k3 */
void des_block_encrypt_3DES_3TDEA(uint8_t * block, uint8_t * key){

	des_block_encrypt(block, &(key[0]));

	des_block_decrypt(block, &(key[DES_BLOCK_BYTE_LEN]));

	des_block_encrypt(block, &(key[DES_BLOCK_BYTE_LEN*2]));

}

void des_block_decrypt_3DES_3TDEA(uint8_t * block, uint8_t * key){

	des_block_decrypt(block, &(key[DES_BLOCK_BYTE_LEN*2]));

	des_block_encrypt(block, &(key[DES_BLOCK_BYTE_LEN]));

	des_block_decrypt(block, &(key[0]));

}

/* Triple length key: k1, k2, k1 (A-B-A) */
void des_block_encrypt_3DES_2TDEA(uint8_t * block, uint8_t * key){

	// Encrypt step (Key A)
	des_block_encrypt(block, key);

	// Decrypt step (Key B)
	des_block_decrypt(block, &(key[DES_BLOCK_BYTE_LEN]));

	// Encrypt step (Key A)
	des_block_encrypt(block, key);

}

void des_block_decrypt_3DES_2TDEA(uint8_t * block, uint8_t * key){

	// Decrypt step (Key A)
	des_block_decrypt(block, key);

	// Encrypt step (Key B)
	des_block_encrypt(block, &(key[DES_BLOCK_BYTE_LEN]));

	// Decrypt step (Key A)
	des_block_decrypt(block, key);

}


void des_block_encrypt(uint8_t * block, uint8_t * key){

	uint8_t rotation_type;
	// Declare larger key than necessary for easier operation
	uint8_t transformed_key[DES_BLOCK_BYTE_LEN] = {0x0};
	// Make a copy of the key
	uint8_t key_cpy[DES_BLOCK_BYTE_LEN];
	memcpy(key_cpy, key, DES_BLOCK_BYTE_LEN);

	// INITIAL BLOCK PERMUTATION
	permute_overwrite_and_flip(block, (const int **)&InitialPermutation, DES_BLOCK_BYTE_LEN);
#ifdef DES_BLOCK_ENCRYPT_DEBUG
	printf("(1) INITIAL (BLOCK) PERMUTATION\n");
	print_array_pretty(block, 8);
#endif

	// INITIAL KEY PERMUTATION
	permute_overwrite_and_flip(key_cpy, (const int **)&PC1, DES_BLOCK_BYTE_LEN);
#ifdef DES_BLOCK_ENCRYPT_DEBUG
	printf("(2) INITIAL KEY PERMUTATION\n");
	print_array_pretty(key, 8);
#endif

	for (uint8_t round_idx = 0; round_idx < NUM_ROUNDS; round_idx++){

		// Rotate key to the LEFT (Key schedule for encryption)
		rotation_type = Rotations[round_idx] - 1;
		permute_overwrite_and_flip(key_cpy, (const int **)&RotateLeft[rotation_type], DES_BLOCK_BYTE_LEN);

#ifdef DES_BLOCK_ENCRYPT_DEBUG
		printf("///////////////////\n    Key rotate at round %d\n", round_idx);
		print_array_pretty(key, 8);
#endif

		permute_and_flip(transformed_key, key_cpy, (const int **)&PC2, DES_BLOCK_BYTE_LEN);
#ifdef DES_BLOCK_ENCRYPT_DEBUG
		printf("    Key transform at round (KS) %d\n", round_idx);
		print_array_pretty(transformed_key, 8);
#endif

		// ROUND i
		des_round( block, transformed_key );
#ifdef DES_BLOCK_ENCRYPT_DEBUG
		printf("    Block at round %d\n", (round_idx+1));
		print_array_pretty(block, 8);
		printf("///////////////////\n");
#endif
	}

	// LAST SWAP
	rightLeftSwap(block);
#ifdef DES_BLOCK_ENCRYPT_DEBUG
	printf("(3) LAST SWAP\n");
	print_array_pretty(block, 8);
#endif

	// FINAL PERMUTATION
	permute_overwrite_and_flip(block, (const int **)&FinalPermutation, DES_BLOCK_BYTE_LEN);
#ifdef DES_BLOCK_ENCRYPT_DEBUG
	printf("(4) FINAL PERMUTATION\n");
	print_array_pretty(block, 8);
#endif

}

void des_block_decrypt(uint8_t * block, uint8_t * key){

	uint8_t rotation_type;
	// Declare larger key than necessary for easier operation
	uint8_t transformed_key[DES_BLOCK_BYTE_LEN] = {0x0};
	// Make a copy of the key
	uint8_t key_cpy[DES_BLOCK_BYTE_LEN];
	memcpy(key_cpy, key, DES_BLOCK_BYTE_LEN);

	// INITIAL BLOCK PERMUTATION
	permute_overwrite_and_flip(block, (const int **)&InitialPermutation, DES_BLOCK_BYTE_LEN);
#ifdef DES_BLOCK_ENCRYPT_DEBUG
	printf("(1) INITIAL (BLOCK) PERMUTATION\n");
	print_array_pretty(block, 8);
#endif

	// INITIAL KEY PERMUTATION
	permute_overwrite_and_flip(key_cpy, (const int **)&PC1, DES_BLOCK_BYTE_LEN);
#ifdef DES_BLOCK_ENCRYPT_DEBUG
	printf("(2) INITIAL KEY PERMUTATION\n");
	print_array_pretty(key, 8);
#endif

	for (uint8_t round_idx = 0; round_idx < NUM_ROUNDS; round_idx++){

		permute_and_flip(transformed_key, key_cpy, (const int **)&PC2, DES_BLOCK_BYTE_LEN);
#ifdef DES_BLOCK_ENCRYPT_DEBUG
		printf("    Key transform at round (KS) %d\n", round_idx);
		print_array_pretty(transformed_key, 8);
#endif

		// ROUND i
		des_round( block, transformed_key );
#ifdef DES_BLOCK_ENCRYPT_DEBUG
		printf("    Block at round %d\n", (round_idx+1));
		print_array_pretty(block, 8);
		printf("///////////////////\n");
#endif

		// Rotate key to the RIGHT (Key schedule for decryption)
		rotation_type = Rotations[NUM_ROUNDS - 1 - round_idx] - 1;
		permute_overwrite_and_flip(key_cpy, (const int **)&RotateRight[rotation_type], DES_BLOCK_BYTE_LEN);

#ifdef DES_BLOCK_ENCRYPT_DEBUG
		printf("///////////////////\n    Key rotate at round %d\n", round_idx);
		print_array_pretty(key, 8);
#endif

	}

	// LAST SWAP
	rightLeftSwap(block);
#ifdef DES_BLOCK_ENCRYPT_DEBUG
	printf("(3) LAST SWAP\n");
	print_array_pretty(block, 8);
#endif

	// FINAL PERMUTATION
	permute_overwrite_and_flip(block, (const int **)&FinalPermutation, DES_BLOCK_BYTE_LEN);
#ifdef DES_BLOCK_ENCRYPT_DEBUG
	printf("(4) FINAL PERMUTATION\n");
	print_array_pretty(block, 8);
#endif

}
