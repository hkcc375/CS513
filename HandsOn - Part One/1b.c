/*
================================================================================================
Name : 1b.c
Author : Hemant Kumar Chandavar
Description : Create the following types of a files using (i) shell command (ii)
system call
a. soft link (symlink system call)
b. hard link (link system call)
c. FIFO (mkfifo Library Function or mknod system call)
Date : 28th August, 2023
================================================================================================
*/

/* The command to create a hard link using shell command --> ln <path to
 * file/folder to be linked> <path of the link to be created> */

#include <fcntl.h>
#include <stdio.h>
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
		{
			printf( "The file was created successfully. \n" );
			if( link( fileName, "hard_link_file" ) < 0 )
				perror( "There was an error while creating a "
				        "hard link. \n" );
			else
				printf( "The hard link was created "
				        "successfully. \n" );
		}
	}
}
