#include <stdio.h>
#include<stdlib.h>
#include <string.h>

struct cpuidCacheInfo {
//EAX
  unsigned int type;
  unsigned int level;
  unsigned int selfInitializing;
  unsigned int fullyAssociative;
  unsigned int reservedOne; 
  unsigned int logIds; //max num addressable IDs for logical processors logical processors
  unsigned int phyIds; //max num addressable IDs for processor cors
//EBX
  unsigned int lineSize;
  unsigned int partitions;
  unsigned int associativity;
//ECX
  unsigned int sets;
//EDX
  unsigned int wbinvd; //write back invalidate/invalidate, if 0, cache info shared to lower levels, if 1, not guaranteed
  unsigned int inclusive; //is this cache inclusive
  unsigned int complexIndex; //direct mapped if 0, 1 means complex function used to index cache (probably for L3 in intel cases)
  unsigned int reservedTwo;
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

void zipStrings(char * combinedStr, char * str1, char * str2) //used to concatenate strings
{
  strcpy(combinedStr, str1);
  strcat(combinedStr, str2);
}

int parseRegs(unsigned int regs[])
{
  int regReturn;

  system("rm -f info"); //cache info stored in here
  system ("touch info");

	char regStr[4][9]; //4 registers, 8 values each (and obligatory '\n') = 9

	int i;
	for(i = 0; i < 4; i++)	
		snprintf(regStr[i], 9, "%08X", regs[i]);

	//printf("\neax: %s ebx: %s ecx: %s edx: %s\n", regStr[0], regStr[1], regStr[2], regStr[3]);	//printing out register output

  int j;
  char temp[3];
  char command[3+6+19];
  char half[6] = "grep "; //setting command ops
  char other[27] = " CacheDecoders.csv >> info";

  for(i = 0; i < 4; i++)
  {
    for(j = 0; j < 8; j+=2)
    {
      temp[0] = regStr[i][j]; //sets temp decode value
      temp[1] = regStr[i][j+1];

      if(strcmp(temp, "FF")) //checking if FF exists, as this means CPUID.4 will need to be used
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

unsigned int isolateInt(unsigned int bits, unsigned int first, unsigned int last) //isolates given integer from first to last
{
  unsigned mask = ( (1<<(last - first + 1))-1) << first; //calculating the mask from a certain range to another (first-last)
  return (bits & mask) >> first; //masks bits then bitshifts back to first
}

int getLeafFourInfo(unsigned int regs[], int currInc)
{
    regs[0] = 4;
    regs[2] = currInc;
    cpuid(regs);  
    if(isolateInt(regs[0], 0, 4) == 0) //checking if eax[0:4] == 0 or not
      return 1;
    else
      return 0; //still getting cache info
}

int numCacheCalc(unsigned int regs[]) //finding how many caches there are to properly malloc cache struct
{
  int c = 0;
  int cpuIDRes;

  while(cpuIDRes != 1)
  { 
    cpuIDRes = getLeafFourInfo(regs, c); 
    if(cpuIDRes != 1) //only runs if eax[0:4] != 0
      c++;  
  }
  return c;
}

struct cpuidCacheInfo *leafFour(unsigned int regs[])
{
  int counter = numCacheCalc(regs);  //gets number of caches to dynamically allocate cache structs
  struct cpuidCacheInfo * cache =(struct cpuidCacheInfo *)malloc(sizeof(struct cpuidCacheInfo*)*counter-1);

  int cpuIDRes; //result from getLeafFourInfo  
  unsigned int isolatedBits;

  int currECX = 0;
  int first = 5;
  int last = 7;  

  while(cpuIDRes != 1)
  { 
    cpuIDRes = getLeafFourInfo(regs, currECX); //gets leaf 4 info

    if(cpuIDRes != 1) //only runs if eax[0:4] != 0
    {        
      isolatedBits = isolateInt(regs[0], first, last);          
      cache[currECX].level = isolatedBits;  
      currECX++;
    } 
  }

  return cache; //returns cache structs
}

int main()
{
    struct cpuidCacheInfo * caches;
    unsigned int regs[4]; //registers EAX, EBX, ECX, EDX

    regs[0] = 2; //getting cache info from CPUID.2, where EAX = 2
    cpuid(regs);

    int useLeafFour = parseRegs(regs);
    if(useLeafFour == 1)
    {
      printf("Using CPUID Leaf 4 for further cache information:");
      caches = leafFour(regs);
      int i;
      int j = numCacheCalc(regs);
      for(i = 0; i < j; i++)
        printf("\n%d", caches[i].level);
    }
    else
      printf("No CPUID Leaf 4");    
		putchar('\n');
    return 0;
}