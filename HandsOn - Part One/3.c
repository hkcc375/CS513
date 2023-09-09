/*
================================================================================================
Name : 3.c
Author : Hemant Kumar Chandavar
Description : Write a program to create a file and print the file descriptor
value. Use creat ( ) system call
Date : 28th August, 2023
================================================================================================
*/

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define RD_WR 0666

int main( int argc, char* argv[] )
{
	char* fileName;
	int fileDescriptor;
	if( argc != 2 )
		printf( "Pass the filename as a command line argument. \n" );
	else
	{
		fileName       = argv[1];
		fileDescriptor = creat( fileName, RD_WR );

		if( fileDescriptor == -1 )
			perror( "Error while creating the file ! \n" );
		else
			printf( "The file was created successfully and the "
			        "file descriptor value is %d. \n",
			        fileDescriptor );
	}
}
