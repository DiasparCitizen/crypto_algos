
#ifndef BLOCKCIPHERS_KASUMI_H_
#define BLOCKCIPHERS_KASUMI_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "../Math/math.h"
#include "../Helper/helper.h"

#define KASUMI_BLOCK_BIT_SZ 64
#define KASUMI_KEY_BIT_SZ 128

#define KASUMI_BLOCK_BYTE_SZ (KASUMI_BLOCK_BIT_SZ / 8)
#define KASUMI_KEY_BYTE_SZ (KASUMI_KEY_BIT_SZ / 8)

void kasumi_test();

void kasumi_block_encrypt(uint8_t * block, uint8_t * key);

#endif /* BLOCKCIPHERS_KASUMI_H_ */
