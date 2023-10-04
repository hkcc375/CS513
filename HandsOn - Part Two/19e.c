#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>

int main(int argc, char* argv[])
{
	char* fileName;
	int retval;
	if(argc != 2)
		printf("Please pass the file name as a command line argument. \n");
	else
	{
		fileName = argv[1];
		retval = mkfifo(fileName, 0744);
		if(retval == 0)
			printf("The FIFO file was created successfully. \n");
		else
			perror("There was an error while creating the FIFO file. \n");
	}
}
