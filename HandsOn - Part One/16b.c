/*
================================================================================================
Name : 16b.c
Author : Hemant Kumar Chandavar
Description :  Write a program to perform mandatory locking.
a. Implement write lock
b. Implement read lock
Date : 28th August, 2023
================================================================================================
*/

#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
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
		char* fileName = argv[1];
		int fileDescriptor;
		struct flock lock;

		lock.l_type   = F_RDLCK;
		lock.l_whence = SEEK_SET;
		lock.l_start  = 0;
		lock.l_len    = 0;
		lock.l_pid    = getpid();

		printf( "I am trying to open the file. \n" );
		fileDescriptor = open( fileName, O_RDWR );
		if( fileDescriptor == -1 )
			perror( "Error while opening a file. \n" );
		else
		{
			printf( "I am going to lock this file using read lock "
			        "now. \n" );
			fcntl( fileDescriptor, F_SETLKW, &lock );
			printf( "The file is now locked using read lock. \n" );
			printf( "Enter to unlock this file.. \n" );
			getchar();
			lock.l_type = F_UNLCK;
			fcntl( fileDescriptor, F_SETLK, &lock );
			printf( "The file is now unlocked. \n" );
		}
	}
}
