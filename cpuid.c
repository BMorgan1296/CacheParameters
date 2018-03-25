#include <stdio.h>
#include<stdlib.h>
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

void zipStrings(char * combinedStr, char * str1, char * str2)
{
  strcpy(combinedStr, str1);
  strcat(combinedStr, str2);
}

char *searchFile(FILE *inputFile, char *searchFor, int n)
{

  char buf[256];
  char *output;
  bzero(buf,256);

  while(fgets(buf, 256, inputFile) != NULL)
  {
    if((strstr(buf, searchFor)) != NULL)
    {
      output = buf;     
      break;
    }   
  }

  if((strstr(buf, searchFor)) == NULL)
    {     
      output = "NOT FOUND";
    }

    return output;
}

void parseRegs(unsigned int regs[])
{
	char regStr[4][9];

	int i;
	for(i = 0; i < 4; i++)	
		snprintf(regStr[i], 9, "%08X", regs[i]);

	printf("eax: %s ebx: %s ecx: %s edx: %s\n", regStr[0], regStr[1], regStr[2], regStr[3]);	

  int j;
  char temp[3];
  char command[3+6+19];
  char half[6] = "grep ";
  char other[19] = " CacheDecoders.csv";

  for(i = 0; i < 4; i++)
  {
    for(j = 0; j < 8; j+=2)
    {
      temp[0] = regStr[i][j];
      temp[1] = regStr[i][j+1];

      zipStrings(command, half, temp);
      zipStrings(command, command, other);

      if(temp[0] == '0' && temp[1] == '0')
      {/*skips values with are 00, as they indicate that there is no information.*/}
      else
      system(command);
      
    }
    
  }
}

int main()
{
    unsigned int regs[4];
    regs[0] = 2;
    native_cpuid(regs);

    parseRegs(regs);
		
    return 0;
}