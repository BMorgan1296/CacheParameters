#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>

int main() //http://www.sandpile.org/x86/cpuid.htm and also https://www.scss.tcd.ie/Jeremy.Jones/CS4021/processor-identification-cpuid-instruction-note.pdf
{
	int32_t op = 2;  // input:  eax
	int32_t eax; // output: eax
	int32_t ebx; // output: ebx
	int32_t ecx; // output: ecx
	int32_t edx; // output: edx
    
	__asm__ volatile(
			"cpuid;" //  call cpuid instruction
			:"=a"(eax),"=b"(ebx),"=c"(ecx), "=d"(edx)// output equal to "movl  %%eax %1"
			:"a"(op)// input equal to "movl %1, %%eax"
			//:"%eax","%ebx","%ecx","%edx"// clobbered register
			);

	printf("\neax: 0x%08X   ebx: 0x%08X   ecx: 0x%08X   edx: 0x%08X", eax, ebx, ecx, edx);
	putchar('\n');
	putchar('\n');
  
}