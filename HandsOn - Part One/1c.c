/*
================================================================================================
Name : 1c.c
Author : Hemant Kumar Chandavar
Description : Create the following types of a files using (i) shell command (ii)
system call
a. soft link (symlink system call)
b. hard link (link system call)
c. FIFO (mkfifo Library Function or mknod system call)
Date : 28th August, 2023
================================================================================================
*/

/* The command to create a fifo file using shell command --> mkfifo <name of
 * fifo filename> */

#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define RD_WR 0666

int main( int argc, char* argv[] )
{
	char* fileName;
	int status;
	if( argc != 2 )
		printf( "Pass the filename as a command line argument. \n" );
	else
	{
		fileName = argv[1];
		status   = mknod( fileName, S_IFIFO | 0666, 0 );
		if( status == -1 )
			perror( "Error while creating the FIFO file ! \n" );
		else { printf( "The FIFO file was created successfully. \n" ); }
	}
}