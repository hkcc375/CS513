// Question 3 : Write a program to create a file and print the file descriptor value. Use the creat() system call.

#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<unistd.h>

#define RD_WR 0666

int main(int argc, char* argv[])
{
	char* filename;
	int fileDescriptor; // FD -> Success, and -1 -> Error
	if(argc != 2)
		printf("Pass the filename as a command line argument. \n");
	else
	{
		filename = argv[1];
		fileDescriptor = creat(filename, RD_WR);
		
		if(fileDescriptor == -1)
			perror("Error while creating the file ! \n");
		else
			printf("The file was created successfully and the file descriptor value is %d. \n", fileDescriptor);
	}
	
}
