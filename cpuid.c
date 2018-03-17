#include <stdio.h>
#include <stdint.h>

void DecToHexStr(int dec, char *str)
  {  }

int main()
{
	int32_t op = 2;  // input:  eax
	int32_t eax; // output: eax
	int32_t ebx; // output: ebx
	int32_t ecx; // output: ecx
	int32_t edx; // output: edx

  asm volatile( "cpuid"
              : "=a"(eax), "=b"(ebx), "=c"(ecx), "=d"(edx)
              : "a" (op) );      

    printf("\neax: %zu\nebx: %zu\necx: %zu\nedx: %zu\n", eax & ((1<<8)-1) ,ebx, ecx & 0xFF,edx);

	  
	for(int i = 0; i < 32; i++)
	{
		printf("\n%d | eax: %zu ebx: %zu ecx: %zu edx: %zu", i, eax & ((1<<i)-1), ebx & ((1<<i)-1), ecx & ((1<<i)-1), edx & ((1<<i)-1));
	}
	putchar('\n');
  
}