#include <stdio.h>

int main()
{
	int val ;

  asm("movl $110, %%ebx;"
      "movl $5, %%eax;"
      "addl %%ebx, %%eax;" :"=r" ( val )
    );

  printf("\nVal: %d\n", val);
}