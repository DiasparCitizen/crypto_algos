
#ifndef TESTS_TEST_H_
#define TESTS_TEST_H_

#include "../Helper/helper.h"
#include "../BlockCiphers/DES.h"

extern int InitialPermutation[64];

void test_AES_GCM_1();

void test_DES_block_cipher_1();

void test_DES_f_function();

#endif /* TESTS_TEST_H_ */
