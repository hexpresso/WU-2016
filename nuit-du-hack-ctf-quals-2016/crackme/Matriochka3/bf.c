#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

uint32_t
f(uint8_t c, int32_t shift, uint32_t n)
{
	int64_t factor;
	
	factor  = 0x3E8;
	factor *= c;
	factor *= n;
	factor >>= 32;
	factor >>= shift;
	// printf("%p\n", factor);
	//printf("%08X\n", top);
	return factor;
}

uint8_t
getChar(int32_t shift, int32_t n)
{
	uint8_t i;
	uint32_t ret;
	
	for(i = 0; i < 0xFF; i++) {
		ret = f(i, shift, n);
		
		if(ret == 0x3E7 || ret == 0x3E8)
			return i;
	}
}

void
main(void)
{
	printf("%c", getChar(5, 0x78787879));
	printf("%c", getChar(6, 0x9C09C09D));
	printf("%c", getChar(5, 0x51EB851F));
	printf("%c", getChar(6, 0xAC769185));
	
	printf("%c", getChar(1, 0x043B3D5B));
	printf("%c", getChar(6, 0x939A85C5));
	printf("%c", getChar(6, 0x8C08C08D));
	printf("%c", getChar(6, 0xAC769185));
	
	printf("%c", getChar(5, 0x4BDA12F7));
	printf("%c", getChar(6, 0x9C09C09D));
	printf("%c", getChar(5, 0x4C8F8D29));
	printf("%c", getChar(4, 0x288DF0CB));
	printf("%c", getChar(6, 0xAC769185));
	
	printf("%c", getChar(5, 0x473C1AB7));
	printf("%c", getChar(6, 0x9C09C09D));
	printf("%c", getChar(3, 0x13E22CBD));
	printf("%c", getChar(2, 0x094F2095));
	printf("%c", getChar(3, 0x151D07EB));
	printf("%c", getChar(5, 0x4BDA12F7));
	printf("%c", getChar(5, 0x473C1AB7));
	printf("%c", getChar(5, 0x82082083));
}