#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

struct cpuinfo
{
	char *info[5];
};

struct cpuinfo currCPU;	

void zipStrings(char * combinedStr, char * str1, char * str2)
{
	strcpy(combinedStr, str1);
	strcat(combinedStr, str2);
}

void searchFile(FILE *inputFile, char *searchFor, int n)
{
	FILE *CPUINFOCPY = inputFile;

	char buf[128];
	char colon[2] = ": ";
	char *temp;
	char *output;

	bzero(buf,128);

	while(fgets(buf, 128, CPUINFOCPY) != NULL)
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

	currCPU.info[n] = strdup(output);	
}

int copyCPUINFO()
{
	char pwd[128];

	getcwd(pwd, 128);	//gets pwd

	char halfCommand[20] = "cat /proc/cpuinfo >"; //half of the system command
	char secHalf[12] = "/cpuinfocpy\0"; //new file to be created after pwd is found

	char notYetCommand[20+128]; //holds halfcommand and pwd
	char sysCommand[20+128+12]; //hold notYetCommand and secHalf

	zipStrings(notYetCommand, halfCommand, pwd); //zips halfcommand and pwd
	zipStrings(sysCommand, notYetCommand, secHalf); //zips these together to make the full system command. Basically cat dir + pwd + new file

	system("touch cpuinfocpy"); //creats new file
	system(sysCommand);	

	return 1;
}



int main() //cat /proc/cpuinfo
{
	FILE *CPUINFOCPY;
	

	if(copyCPUINFO() == 1)
	{
		printf("\nSuccessfully copied /proc/cpuinfo for parsing\n");
	}

	CPUINFOCPY = fopen("cpuinfocpy", "r");
	
	char *search[5];
		search[0] = "vendor_id"; 
		search[1] = "model name";
		search[2] = "cache size";
		search[3] = "cpu cores";
		search[4] = "cache_alignment";	

	int i;
	for(i = 0; i < 5; i++)
	{
		printf("\nSearching for: %s", search[i]);
		searchFile(CPUINFOCPY, search[i], i);		
		printf(" | Found: %s", currCPU.info[i]);
	}	
	
	if(CPUINFOCPY) //closes CPUINFOCPY
	{
		fclose(CPUINFOCPY);
	}

	putchar('\n');
}