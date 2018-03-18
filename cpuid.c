/*#include <stdio.h>
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

	printf("\nShift: %d  |  eax: %X   ebx: 0x%08X   ecx: 0x%08X   edx: 0x%08X", i, (eax & ((1<<i)-1)), (ebx & ((1<<i)-1)), (ecx & ((1<<i)-1)), (edx & ((1<<i)-1)));

	int i;
	for(i = 32; i >= 0; i--)
	{
		//printf("\nShift: %d  |  eax: %zu   ebx: %zu   ecx: %zu   edx: %zu", i, (unsigned long)(eax & ((1<<i)-1)), (unsigned long)(ebx & ((1<<i)-1)), (unsigned long)(ecx & ((1<<i)-1)), (unsigned long)(edx & ((1<<i)-1)));
		
		printf("\nShift: %d  |  eax: 0x%08X   ebx: 0x%08X   ecx: 0x%08X   edx: 0x%08X", i, (eax & ((1<<i)-1)), (ebx & ((1<<i)-1)), (ecx & ((1<<i)-1)), (edx & ((1<<i)-1)));
		
		if(i % 8 == 0 && i != 0)
		{
			printf("\n\n------------------------ Byte: %d --------------------------\n", (i/8));
		}
	}
	putchar('\n');
	putchar('\n');
  
}*/

#include <stdio.h>
#include <string.h>

static inline void native_cpuid(unsigned int *eax, unsigned int *ebx,
                 unsigned int *ecx, unsigned int *edx)
{
    
    asm volatile("cpuid"
        : "=a" (*eax),
        "=b" (*ebx),
        "=c" (*ecx),
        "=d" (*edx)
        : "0" (*eax), "2" (*ecx)
        : "memory");
}

int main()
{
    int eax, ebx, ecx, edx;
    eax = 0x80000006;
    native_cpuid(&eax, &ebx, &ecx, &edx);
    printf("\nL2 Cache Size = %d\n", ecx & ((1<<14)-1)); //see https://www.scss.tcd.ie/Jeremy.Jones/CS4021/processor-identification-cpuid-instruction-note.pdf PAGE 47
    printf("\n12 = %08X\n", (ecx & ((1<<12)-1)));
    printf("\n13 = %08X\n", (ecx & ((1<<13)-1)));
    printf("\n14 = %08X\n", (ecx & ((1<<14)-1)));
    printf("\n15 = %08X\n", (ecx & ((1<<15)-1)));
    printf("\n16 = %08X\n", (ecx & ((1<<16)-1)));
    printf("\n17 = %08X\n", (ecx & ((1<<17)-1)));
    printf("\n18 = %08X\n", (ecx & ((1<<18)-1)));
    return 0;
}