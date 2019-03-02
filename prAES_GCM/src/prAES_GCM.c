
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "Math/math.h"
#include "BlockCiphers/AES.h"
#include "TextCiphers/textciphers.h"
#include "Tests/test.h"

#define BLOCK_SIZE_BYTES	16





int main(void) {

	//http://testprotect.com/appendix/AEScalc
	test_AES_GCM_1();



	/*uint8_t region_x[16];
	uint8_t region_y[16];
	uint8_t region_z[16];
	memset(region_x, 0x0, 16);
	memset(region_y, 0x0, 16);
	memset(region_z, 0x0, 16);

	//region_x[0] = 0b10100000;
	//region_y[0] = 0b11100000;

	region_x[0] = 0b00000101;
	region_y[0] = 0b00000000;

	print_region(region_x);
	print_region(region_y);
	print_region(region_z);

	printf("GALOIS!\n");
	galois_128_mult_lbe(region_x, region_y, region_z);
	print_region(region_z);*/

	return EXIT_SUCCESS;

}
