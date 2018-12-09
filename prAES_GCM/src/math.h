/*
 * math.h
 *
 *  Created on: 7 dic. 2018
 *      Author: Ich
 */

#ifndef MATH_H_
#define MATH_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

void galois_128_mult_lle(uint8_t *region_x, uint8_t *region_y, uint8_t *region_z);
void print_region(uint8_t *region);

#endif /* MATH_H_ */
