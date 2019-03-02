/*
 * AES.h
 *
 *  Created on: 27 feb. 2019
 *      Author: Ich
 */

#ifndef AES_H_
#define AES_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "../Math/math.h"

//#define DEBUG
#define BLOCK_SIZE_BYTES	16

void encrypt_block_AES(uint8_t *block, uint8_t *key);

#endif /* AES_H_ */
