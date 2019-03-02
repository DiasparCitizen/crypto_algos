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

void encrypt_data_AES_GCM(uint8_t *data, int data_size, uint8_t key[BLOCK_SIZE_BYTES], uint8_t iv[12], uint8_t *aad);

#endif /* TEXTCIPHERS_TEXTCIPHERS_H_ */
