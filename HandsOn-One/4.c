/* Question : Write a program to open an existing file with read write mode. Try O_EXCL flag also.
*/

#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>

#define RD_WR 0666

int main(int argc, char* argv[])
{
	char* filename;
	int fileDescriptor;
	if(argc != 2)
		printf("Pass the filename as a command line argument. \n");
	else
	{
		filename = argv[1];
		fileDescriptor = open(filename, O_CREAT|O_RDWR, RD_WR);
		if(fileDescriptor == -1)
			perror("Error while opening an existing file ! \n");
		else
			printf("The file was opened successully in read-write mode. \n");
	}
	
}
