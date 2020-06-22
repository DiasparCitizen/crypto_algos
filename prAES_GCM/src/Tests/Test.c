
#include "Test.h"

//https://paginas.fe.up.pt/~ei10109/ca/des.html
//https://academic.csuohio.edu/yuc/security/Chapter_06_Data_Encription_Standard.pdf
//https://www.commonlounge.com/discussion/5c7c2828bf6b4724b806a9013a5a4b99
//http://people.duke.edu/~tkb13/courses/ncsu-csc405-2015fa/RESOURCES/JS-DES.shtml

void random_test(){

	kasumi_test();
}

uint8_t check_output_correctness(const uint8_t *output, const uint8_t *expected_output, uint16_t byte_size){

	uint8_t correct = 1;
	for (uint16_t byte_idx = 0; byte_idx < byte_size; byte_idx++){
		//printf("loop");
		if (output[byte_idx] != expected_output[byte_idx]){
			correct = 0;
			break;
		}
	}

	if (correct){
		printf(">> TEST CORRECT: output matches expected output! <<\n");
	}else{
		printf(">> TEST FAILED: output does NOT match expected output! <<\n");
	}

	return correct;

}

void test_AES_GCM_1(){

	// Ver vectores y resultados en:
	// The Galois/Counter mode of operation (GCM), por David A. McGrew y John Viega
	// P�gina 28

	uint8_t key[16] = {0xfe, 0xff, 0xe9, 0x92, 0x86, 0x65, 0x73, 0x1c, 0x6d, 0x6a, 0x8f, 0x94, 0x67, 0x30, 0x83, 0x08};
	uint8_t plaintext[64] = {0xd9, 0x31, 0x32, 0x25, 0xf8, 0x84, 0x06, 0xe5, 0xa5, 0x59, 0x09, 0xc5, 0xaf, 0xf5, 0x26, 0x9a,
			0x86, 0xa7, 0xa9, 0x53, 0x15, 0x34, 0xf7, 0xda, 0x2e, 0x4c, 0x30, 0x3d, 0x8a, 0x31, 0x8a, 0x72,
			0x1c, 0x3c, 0x0c, 0x95, 0x95, 0x68, 0x09, 0x53, 0x2f, 0xcf, 0x0e, 0x24, 0x49, 0xa6, 0xb5, 0x25,
			0xb1, 0x6a, 0xed, 0xf5, 0xaa, 0x0d, 0xe6, 0x57, 0xba, 0x63, 0x7b, 0x39, 0x1a, 0xaf, 0xd2, 0x55};
	uint8_t iv[12] = {0xca, 0xfe, 0xba, 0xbe, 0xfa, 0xce, 0xdb, 0xad, 0xde, 0xca, 0xf8, 0x88};
	//feed face dead beef feed face dead beef abad dad2
	//uint8_t aad[20] = {0xfe, 0xed, 0xfa, 0xce, 0xde, 0xad, 0xbe, 0xef, 0xfe, 0xed, 0xfa, 0xce, 0xde, 0xad, 0xbe, 0xef, 0xab, 0xad, 0xda, 0xd2};
	uint8_t aad[20] = {0x0};
	uint8_t tag[16] = {0x0};

	encrypt_data_AES_GCM(plaintext, 64, key, iv, aad, 0, tag);

	printf("\nCipher text:\n");
	print_array_pretty(plaintext, 64);
	printf("Tag:\n");
	print_array_pretty(tag, 16);

}

void test_GCM_MULT(){

	printf("\n************** test_GCM_MULT()\n");

	// 0x7b, 0x5b, 0x54, 0x65, 0x73, 0x74, 0x56, 0x65, 0x63, 0x74, 0x6f, 0x72, 0x5d, 0x53, 0x47, 0x5d
	uint8_t A[16] = {0x7b, 0x5b, 0x54, 0x65, 0x73, 0x74, 0x56, 0x65, 0x63, 0x74, 0x6f, 0x72, 0x5d, 0x53, 0x47, 0x5d};
	// 0x48, 0x69, 0x28, 0x53, 0x68, 0x61, 0x79, 0x29, 0x5b, 0x47, 0x75, 0x65, 0x72, 0x6f, 0x6e, 0x5d
	uint8_t B[16] = {0x48, 0x69, 0x28, 0x53, 0x68, 0x61, 0x79, 0x29, 0x5b, 0x47, 0x75, 0x65, 0x72, 0x6f, 0x6e, 0x5d};
	// 0x04, 0x02, 0x29, 0xa0, 0x9a, 0x5e, 0xd1, 0x2e, 0x7e, 0x4e, 0x10, 0xda, 0x32, 0x35, 0x06, 0xd2
	uint8_t OUT[16] = {0x0};
	uint8_t expected_output[16] = {0x04, 0x02, 0x29, 0xa0, 0x9a, 0x5e, 0xd1, 0x2e, 0x7e, 0x4e, 0x10, 0xda, 0x32, 0x35, 0x06, 0xd2};

	galois_128_mult_lle_reverse(A, B, OUT);
	printf("Result:\n");
	print_array_pretty(OUT, 16);

	check_output_correctness(OUT, expected_output, 16);

}

void test_GCM_MULT_1(){

	printf("\n************** test_GCM_MULT_1()\n");

	//0x952b2a56a5604ac0b32b6656a05b40b6
	uint8_t A[16] = {0x95, 0x2b, 0x2a, 0x56, 0xa5, 0x60, 0x4a, 0xc0, 0xb3, 0x2b, 0x66, 0x56, 0xa0, 0x5b, 0x40, 0xb6};
	//0xdfa6bf4ded81db03ffcaff95f830f061
	uint8_t B[16] = {0xdf, 0xa6, 0xbf, 0x4d, 0xed, 0x81, 0xdb, 0x03, 0xff, 0xca, 0xff, 0x95, 0xf8, 0x30, 0xf0, 0x61};
	// 0xda, 0x53, 0xeb, 0x0a, 0xd2, 0xc5, 0x5b, 0xb6, 0x4f, 0xc4, 0x80, 0x2c, 0xc3, 0xfe, 0xda, 0x60
	uint8_t OUT[16] = {0x0};
	uint8_t expected_output[16] = {0xda, 0x53, 0xeb, 0x0a, 0xd2, 0xc5, 0x5b, 0xb6, 0x4f, 0xc4, 0x80, 0x2c, 0xc3, 0xfe, 0xda, 0x60};

	//galois_128_mult_lbe_new2(A, B, OUT);
	galois_128_mult_lle(A, B, OUT);
	printf("Result:\n");
	print_array_pretty(OUT, 16);

	check_output_correctness(OUT, expected_output, 16);

}

void test_GCM_MULT_2(){

	printf("\n************** test_GCM_MULT_2()\n");

	//0x03 88 DA CE 60 B6 A3 92 F3 28 C2 B9 71 B2 FE 78
	uint8_t A[16] = {0x03, 0x88, 0xDA, 0xCE, 0x60, 0xB6, 0xA3, 0x92, 0xF3, 0x28, 0xC2, 0xB9, 0x71, 0xB2, 0xFE, 0x78};
	//0x66 E9 4B D4 EF 8A 2C 3B 88 4C FA 59 CA 34 2B 2E
	uint8_t B[16] = {0x66, 0xE9, 0x4B, 0xD4, 0xEF, 0x8A, 0x2C, 0x3B, 0x88, 0x4C, 0xFA, 0x59, 0xCA, 0x34, 0x2B, 0x2E};
	//0x5E, 0x2E, 0xC7, 0x46, 0x91, 0x70, 0x62, 0x88, 0x2C, 0x85, 0xB0, 0x68, 0x53, 0x53, 0xDE, 0xB7
	uint8_t OUT[16] = {0x0};
	uint8_t expected_output[16] = {0x5E, 0x2E, 0xC7, 0x46, 0x91, 0x70, 0x62, 0x88, 0x2C, 0x85, 0xB0, 0x68, 0x53, 0x53, 0xDE, 0xB7};

	galois_128_mult_lle_reverse(A, B, OUT);
	printf("Result:\n");
	print_array_pretty(OUT, 16);

	check_output_correctness(OUT, expected_output, 16);

}

/********************************************/
/*************** ECB ************************/
/********************************************/

// NIST p�gina 24, Cipher Modes of Operation
void test_electronic_code_book_ECB(){

	printf("\n************** test_electronic_code_book_ECB()\n");

	// Input:
	// 6bc1bee22e409f96e93d7e117393172a
	// ae2d8a571e03ac9c9eb76fac45af8e51
	// 30c81c46a35ce411e5fbc1191a0a52ef
	// f69f2445df4f9b17ad2b417be66c3710
	// Expected output
	// 3ad77bb40d7a3660a89ecaf32466ef97
	// f5d3d58503b9699de785895a96fdbaaf
	// 43b1cd7f598ece23881b00e3ed030688
	// 7b0c785e27e8ad3f8223207104725dd4

	uint8_t input[64] = {0x6b, 0xc1, 0xbe, 0xe2, 0x2e, 0x40, 0x9f, 0x96, 0xe9, 0x3d, 0x7e, 0x11, 0x73, 0x93, 0x17, 0x2a,
			0xae, 0x2d, 0x8a, 0x57, 0x1e, 0x03, 0xac, 0x9c, 0x9e, 0xb7, 0x6f, 0xac, 0x45, 0xaf, 0x8e, 0x51,
			0x30, 0xc8, 0x1c, 0x46, 0xa3, 0x5c, 0xe4, 0x11, 0xe5, 0xfb, 0xc1, 0x19, 0x1a, 0x0a, 0x52, 0xef,
			0xf6, 0x9f, 0x24, 0x45, 0xdf, 0x4f, 0x9b, 0x17, 0xad, 0x2b, 0x41, 0x7b, 0xe6, 0x6c, 0x37, 0x10};
	uint8_t output[64] = {0x0};
	uint8_t expected_output[64] = {0x3a, 0xd7, 0x7b, 0xb4, 0x0d, 0x7a, 0x36, 0x60, 0xa8, 0x9e, 0xca, 0xf3, 0x24, 0x66, 0xef, 0x97,
			0xf5, 0xd3, 0xd5, 0x85, 0x03, 0xb9, 0x69, 0x9d, 0xe7, 0x85, 0x89, 0x5a, 0x96, 0xfd, 0xba, 0xaf,
			0x43, 0xb1, 0xcd, 0x7f, 0x59, 0x8e, 0xce, 0x23, 0x88, 0x1b, 0x00, 0xe3, 0xed, 0x03, 0x06, 0x88,
			0x7b, 0x0c, 0x78, 0x5e, 0x27, 0xe8, 0xad, 0x3f, 0x82, 0x23, 0x20, 0x71, 0x04, 0x72, 0x5d, 0xd4};

	uint8_t key[16] = {0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6, 0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c};

	printf("\nPlain text:\n");
	print_array_pretty(input, 64);

	//encrypt_ECB(uint8_t *input_data, uint32_t data_byte_len, uint8_t *output_data, uint8_t *key, uint16_t key_byte_len, void (*block_cipher)(), uint16_t block_byte_len);
	encrypt_ECB(input, 64, output, key, 16, &encrypt_block_AES, 16);

	printf("Cipher text:\n");
	print_array_pretty(output, 64);

	check_output_correctness(output, expected_output, 64);

}

/********************************************/
/*************** CBC ************************/
/********************************************/

// NIST p�gina 27, Cipher Modes of Operation
void test_cipher_block_chaining_CBC(){

	printf("\n************** test_cipher_block_chaining_CBC()\n");

	// Input:
	// 6bc1bee22e409f96e93d7e117393172a
	// ae2d8a571e03ac9c9eb76fac45af8e51
	// 30c81c46a35ce411e5fbc1191a0a52ef
	// f69f2445df4f9b17ad2b417be66c3710
	// Expected output
	// 7649abac8119b246cee98e9b12e9197d
	// 5086cb9b507219ee95db113a917678b2
	// 73bed6b8e3c1743b7116e69e22229516
	// 3ff1caa1681fac09120eca307586e1a7

	uint8_t input[64] = {0x6b, 0xc1, 0xbe, 0xe2, 0x2e, 0x40, 0x9f, 0x96, 0xe9, 0x3d, 0x7e, 0x11, 0x73, 0x93, 0x17, 0x2a,
			0xae, 0x2d, 0x8a, 0x57, 0x1e, 0x03, 0xac, 0x9c, 0x9e, 0xb7, 0x6f, 0xac, 0x45, 0xaf, 0x8e, 0x51,
			0x30, 0xc8, 0x1c, 0x46, 0xa3, 0x5c, 0xe4, 0x11, 0xe5, 0xfb, 0xc1, 0x19, 0x1a, 0x0a, 0x52, 0xef,
			0xf6, 0x9f, 0x24, 0x45, 0xdf, 0x4f, 0x9b, 0x17, 0xad, 0x2b, 0x41, 0x7b, 0xe6, 0x6c, 0x37, 0x10};
	uint8_t output[64] = {0x0};
	uint8_t expected_output[64] = {0x76, 0x49, 0xab, 0xac, 0x81, 0x19, 0xb2, 0x46, 0xce, 0xe9, 0x8e, 0x9b, 0x12, 0xe9, 0x19, 0x7d,
			0x50, 0x86, 0xcb, 0x9b, 0x50, 0x72, 0x19, 0xee, 0x95, 0xdb, 0x11, 0x3a, 0x91, 0x76, 0x78, 0xb2,
			0x73, 0xbe, 0xd6, 0xb8, 0xe3, 0xc1, 0x74, 0x3b, 0x71, 0x16, 0xe6, 0x9e, 0x22, 0x22, 0x95, 0x16,
			0x3f, 0xf1, 0xca, 0xa1, 0x68, 0x1f, 0xac, 0x09, 0x12, 0x0e, 0xca, 0x30, 0x75, 0x86, 0xe1, 0xa7};

	uint8_t key[16] = {0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6, 0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c};
	uint8_t iv[16] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f};

	printf("\nPlain text:\n");
	print_array_pretty(input, 64);

	//encrypt_ECB(uint8_t *input_data, uint32_t data_byte_len, uint8_t *output_data, uint8_t *key, uint16_t key_byte_len, void (*block_cipher)(), uint16_t block_byte_len);
	encrypt_CBC(input, 64, output, key, 16, iv, 16, &encrypt_block_AES, 16);

	printf("Cipher text:\n");
	print_array_pretty(output, 64);

	check_output_correctness(output, expected_output, 64);

}

/********************************************/
/*************** OFB ************************/
/********************************************/

// NIST p�gina 27, Cipher Modes of Operation
void test_output_feedback_OFB(){

	printf("\n************** test_output_feedback_OFB()\n");

	// Input:
	// 6bc1bee22e409f96e93d7e117393172a
	// ae2d8a571e03ac9c9eb76fac45af8e51
	// 30c81c46a35ce411e5fbc1191a0a52ef
	// f69f2445df4f9b17ad2b417be66c3710
	// Expected output
	// 3b3fd92eb72dad20333449f8e83cfb4a
	// 7789508d16918f03f53c52dac54ed825
	// 9740051e9c5fecf64344f7a82260edcc
	// 304c6528f659c77866a510d9c1d6ae5e

	uint8_t input[64] = {0x6b, 0xc1, 0xbe, 0xe2, 0x2e, 0x40, 0x9f, 0x96, 0xe9, 0x3d, 0x7e, 0x11, 0x73, 0x93, 0x17, 0x2a,
			0xae, 0x2d, 0x8a, 0x57, 0x1e, 0x03, 0xac, 0x9c, 0x9e, 0xb7, 0x6f, 0xac, 0x45, 0xaf, 0x8e, 0x51,
			0x30, 0xc8, 0x1c, 0x46, 0xa3, 0x5c, 0xe4, 0x11, 0xe5, 0xfb, 0xc1, 0x19, 0x1a, 0x0a, 0x52, 0xef,
			0xf6, 0x9f, 0x24, 0x45, 0xdf, 0x4f, 0x9b, 0x17, 0xad, 0x2b, 0x41, 0x7b, 0xe6, 0x6c, 0x37, 0x10};
	uint8_t output[64] = {0x0};
	uint8_t expected_output[64] = {0x3b, 0x3f, 0xd9, 0x2e, 0xb7, 0x2d, 0xad, 0x20, 0x33, 0x34, 0x49, 0xf8, 0xe8, 0x3c, 0xfb, 0x4a,
			0x77, 0x89, 0x50, 0x8d, 0x16, 0x91, 0x8f, 0x03, 0xf5, 0x3c, 0x52, 0xda, 0xc5, 0x4e, 0xd8, 0x25,
			0x97, 0x40, 0x05, 0x1e, 0x9c, 0x5f, 0xec, 0xf6, 0x43, 0x44, 0xf7, 0xa8, 0x22, 0x60, 0xed, 0xcc,
			0x30, 0x4c, 0x65, 0x28, 0xf6, 0x59, 0xc7, 0x78, 0x66, 0xa5, 0x10, 0xd9, 0xc1, 0xd6, 0xae, 0x5e};

	uint8_t key[16] = {0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6, 0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c};
	uint8_t iv[16] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f};

	printf("\nPlain text:\n");
	print_array_pretty(input, 64);

	//encrypt_ECB(uint8_t *input_data, uint32_t data_byte_len, uint8_t *output_data, uint8_t *key, uint16_t key_byte_len, void (*block_cipher)(), uint16_t block_byte_len);
	encrypt_OFB(input, 64, output, key, 16, iv, 16, &encrypt_block_AES, 16);

	printf("Cipher text:\n");
	print_array_pretty(output, 64);

	check_output_correctness(output, expected_output, 64);

}

/********************************************/
/*************** CTR ************************/
/********************************************/

void test_counter_mode_CTR(){

	printf("\n************** test_counter_mode_CTR()\n");

	// Input:
	// 6bc1bee22e409f96e93d7e117393172a
	// ae2d8a571e03ac9c9eb76fac45af8e51
	// 30c81c46a35ce411e5fbc1191a0a52ef
	// f69f2445df4f9b17ad2b417be66c3710
	// Expected output
	// 874d6191b620e3261bef6864990db6ce
	// 9806f66b7970fdff8617187bb9fffdff
	// 5ae4df3edbd5d35e5b4f09020db03eab
	// 1e031dda2fbe03d1792170a0f3009cee

	uint8_t input[64] = {0x6b, 0xc1, 0xbe, 0xe2, 0x2e, 0x40, 0x9f, 0x96, 0xe9, 0x3d, 0x7e, 0x11, 0x73, 0x93, 0x17, 0x2a,
			0xae, 0x2d, 0x8a, 0x57, 0x1e, 0x03, 0xac, 0x9c, 0x9e, 0xb7, 0x6f, 0xac, 0x45, 0xaf, 0x8e, 0x51,
			0x30, 0xc8, 0x1c, 0x46, 0xa3, 0x5c, 0xe4, 0x11, 0xe5, 0xfb, 0xc1, 0x19, 0x1a, 0x0a, 0x52, 0xef,
			0xf6, 0x9f, 0x24, 0x45, 0xdf, 0x4f, 0x9b, 0x17, 0xad, 0x2b, 0x41, 0x7b, 0xe6, 0x6c, 0x37, 0x10};
	uint8_t output[64] = {0x0};
	uint8_t expected_output[64] = {0x87, 0x4d, 0x61, 0x91, 0xb6, 0x20, 0xe3, 0x26, 0x1b, 0xef, 0x68, 0x64, 0x99, 0x0d, 0xb6, 0xce,
			0x98, 0x06, 0xf6, 0x6b, 0x79, 0x70, 0xfd, 0xff, 0x86, 0x17, 0x18, 0x7b, 0xb9, 0xff, 0xfd, 0xff,
			0x5a, 0xe4, 0xdf, 0x3e, 0xdb, 0xd5, 0xd3, 0x5e, 0x5b, 0x4f, 0x09, 0x02, 0x0d, 0xb0, 0x3e, 0xab,
			0x1e, 0x03, 0x1d, 0xda, 0x2f, 0xbe, 0x03, 0xd1, 0x79, 0x21, 0x70, 0xa0, 0xf3, 0x00, 0x9c, 0xee};

	uint8_t key[16] = {0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6, 0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c};
	uint8_t iv[16] = {0xf0, 0xf1, 0xf2, 0xf3, 0xf4, 0xf5, 0xf6, 0xf7, 0xf8, 0xf9, 0xfa, 0xfb, 0xfc, 0xfd, 0xfe, 0xff};

	printf("\nPlain text:\n");
	print_array_pretty(input, 64);

	//encrypt_ECB(uint8_t *input_data, uint32_t data_byte_len, uint8_t *output_data, uint8_t *key, uint16_t key_byte_len, void (*block_cipher)(), uint16_t block_byte_len);
	encrypt_CTR(input, 64, output, key, 16, iv, 16, 2, &encrypt_block_AES, 16);

	printf("Cipher text:\n");
	print_array_pretty(output, 64);

	check_output_correctness(output, expected_output, 64);

}

/********************************************/
/*************** KASUMI *********************/
/********************************************/

void test_kasumi(){

}


/********************************************/
/*************** DES ************************/
/********************************************/

void test_3DES_2TDEA(){
	// String "0" (ASCII for 0 = 48 dec)
	//uint8_t block[16] = {0x30, 0x0, 0x0, 0x0, 0x00, 0x00, 0x00, 0x00};


	//uint8_t block[16] = {0x89, 0xab, 0xcd, 0xef, 0x00, 0x00, 0x00, 0x00};
	uint8_t block[8] = {0x34, 0x56, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
	uint8_t key[24] = {0x34, 0x56, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xaa, 0xaa, 0x00, 0x00, 0x00, 0xaa, 0x00, 0x00, 0x0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
	//uint8_t key[8] = {0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA};
	printf("decrypt with 2tdea\n");
	des_block_encrypt_3DES_2TDEA(block, key);
	print_array_pretty(block, 8);
	des_block_decrypt_3DES_2TDEA(block, key);
	print_array_pretty(block, 8);
}

void test_3DES_3TDEA(){
	// String "0" (ASCII for 0 = 48 dec)
	//uint8_t block[16] = {0x30, 0x0, 0x0, 0x0, 0x00, 0x00, 0x00, 0x00};


	//uint8_t block[16] = {0x89, 0xab, 0xcd, 0xef, 0x00, 0x00, 0x00, 0x00};
	uint8_t block[8] = {0x34, 0x56, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
	uint8_t key[24] = {0x34, 0x56, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xaa, 0xaa, 0x00, 0x00, 0x00, 0xaa, 0x00, 0x00, 0x0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
	//uint8_t key[8] = {0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA};
	printf("decrypt with 3tdea\n");
	des_block_encrypt_3DES_3TDEA(block, key);
	print_array_pretty(block, 8);
	des_block_decrypt_3DES_3TDEA(block, key);
	print_array_pretty(block, 8);
}

void test_DES_f_function(){
	// String "0" (ASCII for 0 = 48 dec)
	//uint8_t block[16] = {0x30, 0x0, 0x0, 0x0, 0x00, 0x00, 0x00, 0x00};


	//uint8_t block[16] = {0x89, 0xab, 0xcd, 0xef, 0x00, 0x00, 0x00, 0x00};
	uint8_t block[8] = {0x34, 0x56, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
	uint8_t key[8] = {0x33, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
	//uint8_t key[8] = {0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA};
	des_block_encrypt(block, key);
	print_array_pretty(block, 8);
	printf("\n\n*********************\n\n");
	uint8_t key1[8] = {0x33, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
	des_block_decrypt(block, key1);
	print_array_pretty(block, 8);
}
