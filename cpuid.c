#include <stdio.h>
#include<stdlib.h>
#include <string.h>

struct cpuidCacheInfo {
  unsigned int type; //1 cache, 2 tlb, 3 dtlb, 4 stlb?
  unsigned int level;
  unsigned int selfInitializing;
  unsigned int fullyAssociative;
  unsigned int reserved1;
  unsigned int logIds;
  unsigned int phyIds;

  unsigned int lineSize;
  unsigned int partitions;
  unsigned int associativity;

  unsigned int sets;

  unsigned int wbinvd;
  unsigned int inclusive;
  unsigned int complexIndex;
  unsigned int reserved;
};

static inline void cpuid(unsigned int regs[])
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

int parseRegs(unsigned int regs[])
{
  int regReturn;

  system("rm -f info");
  system ("touch info");

	char regStr[4][9];

	int i;
	for(i = 0; i < 4; i++)	
		snprintf(regStr[i], 9, "%08X", regs[i]);

	//printf("\neax: %s ebx: %s ecx: %s edx: %s\n\n", regStr[0], regStr[1], regStr[2], regStr[3]);	//printing out register output

  int j;
  char temp[3];
  char command[3+6+19];
  char half[6] = "grep ";
  char other[27] = " CacheDecoders.csv >> info";

  for(i = 0; i < 4; i++)
  {
    for(j = 0; j < 8; j+=2)
    {
      temp[0] = regStr[i][j];
      temp[1] = regStr[i][j+1];

      if(strcmp(temp, "FF")) //checking if FF exists
        regReturn = 1;
      else
        regReturn = 0;

      zipStrings(command, half, temp); //making the command
      zipStrings(command, command, other);

      if(temp[0] == '0' && temp[1] == '0')
      {/*skips values with are 00, as they indicate that there is no information.*/}
      else
      system(command); //runs grep command
    }    
  }
  putchar('\n');

  return regReturn;
}

int leafFour(unsigned int regs[])
{
  int j;
  int currECX = 0;
  int var;
  while(regs[0] != 0)
  {    
    regs[0] = 4;
    regs[2] = currECX;
    cpuid(regs);

    var = regs[0];
                // printf("\neax: %08d ebx: %08d ecx: %08d edx: %08d", regs[0] & 0x01/*& 0x1F*/, regs[1], regs[2], regs[3]);  //printing out register output
    for(j = 0; j < 32; j++)
    {
      printf("%d", var & 0x01);
      var = var >> 1;
    }
    putchar('\n');
    
    currECX++;
  } 
  putchar('\n');
  putchar('\n');
    return 0;
}

int main()
{
    unsigned int regs[4];
    regs[0] = 2;
    cpuid(regs);

    int useLeafFour = parseRegs(regs);

    
    printf("%d\n", useLeafFour);

    if(useLeafFour == 1)
    {
      printf("\nUsing CPUID Leaf 4 for further cache information:\n");
      leafFour(regs);
    }

    
		
    return 0;
}