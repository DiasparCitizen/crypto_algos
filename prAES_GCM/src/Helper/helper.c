
#include "helper.h"

#define BLOCK_SIZE_BYTES	16


void print_array(uint8_t *text, int size){

	for (int i = 0; i < size; i++){
		printf("0x%x ", text[/*size-i-1*/i]);
	}
	printf("\n");

}

void print_array_pretty(uint8_t *text, int size){

	for (int i = 0; i < size; i++){
		printf("%02X ", text[/*size-i-1*/i]);
		if ( i > 0 && ((i+1) % BLOCK_SIZE_BYTES) == 0 )
			printf("\n");
	}
	printf("\n");

}

void print_array_pretty_se(uint8_t *text, int size){

	for (int i = (size - 1); i >= 0; i--){
		printf("%02X ", text[/*size-i-1*/i]);
		if ( i > 0 && ((i+1) % BLOCK_SIZE_BYTES) == 0 )
			printf("\n");
	}
	printf("\n");

}
