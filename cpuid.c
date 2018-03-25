#include <stdio.h>
#include <string.h>

static inline void native_cpuid(unsigned int regs[])
{    
    asm volatile("cpuid"
        : "=a" (regs[0]),
          "=b" (regs[1]),
          "=c" (regs[2]),
          "=d" (regs[3])
        : "0" (regs[0]), "2" (regs[2])
        : "memory");
}

void parseRegs(unsigned int regs[])
{
	char regStr[4][8];

	int i;
	for(i = 0; i < 4; i++)	
		snprintf(regStr[i], 8, "%08X", regs[i]);

	printf("eaxStr: %s ebxStr: %s ecxStr: %s edxStr: %s\n", regStr[0], regStr[1], regStr[2], regStr[3]);	
}

int main()
{
    unsigned int regs[4];
    regs[0] = 2;
    native_cpuid(regs);

    parseRegs(regs);
		
    return 0;
}