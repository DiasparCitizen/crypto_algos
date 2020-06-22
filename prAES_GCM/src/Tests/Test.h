
#ifndef TESTS_TEST_H_
#define TESTS_TEST_H_

#include "../Helper/helper.h"
#include "../BlockCiphers/DES.h"
#include "../BlockCiphers/AES.h"
#include "../BlockCiphers/Kasumi.h"
#include "../TextCiphers/textciphers.h"

extern int InitialPermutation[64];

void test_AES_GCM_1();

void test_DES_block_cipher_1();

void test_DES_f_function();

void test_GCM_MULT();
void test_GCM_MULT_1();
void test_GCM_MULT_2();

void test_electronic_code_book_ECB();
void test_cipher_block_chaining_CBC();
void test_output_feedback_OFB();
void test_counter_mode_CTR();

void random_test();

#endif /* TESTS_TEST_H_ */
