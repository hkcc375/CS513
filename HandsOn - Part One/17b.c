/*
=============================================================================================
Name : 17b.c
Author : Hemant Kumar Chandavar
Description : Write a program to simulate online ticket reservation. Implement
write lock Write a program to open a file, store a ticket number and exit. Write
a separate program, to open the file, implement write lock, read the ticket
number, increment the number and print the new ticket number then close the
file.
Date : 28th August, 2023
=============================================================================================
*/

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define BUFFER_SIZE 1024

int main( int argc, char* argv[] )
{
	char* fileName;
	int fileDescriptor;
	if( argc != 2 )
	{
		perror( "Pass the file name as a command line arguments. \n" );
	}
	else
	{
		fileName = argv[1];
		struct flock lock;

		lock.l_type   = F_WRLCK;
		lock.l_whence = SEEK_SET;
		lock.l_start  = 0;
		lock.l_len    = 0;
		lock.l_pid    = getpid();

		char buff_write[10];
		char buff_read[BUFFER_SIZE];

		printf( "I am now going to open the file specified in the "
		        "command line argument. \n" );
		fileDescriptor = open( fileName, O_RDWR );
		if( fileDescriptor == -1 )
		{
			perror( "Error while opening the file. \n" );
		}
		else
		{
			printf( "I am now going to lock this file "
			        "using write lock. \n" );
			fcntl( fileDescriptor, F_SETLKW, &lock );
			printf( "The file is now locked using write "
			        "lock. \n" );

			int bufferIndex = 0;
			int readBytes;
			lseek( fileDescriptor, 0, SEEK_SET );

			// Reading the first line of the file
			while( ( readBytes =
			             read( fileDescriptor,
			                   buff_read + bufferIndex, 1 ) ) > 0 )
			{
				if( buff_read[bufferIndex] == '\n' ) break;
				bufferIndex++;
			}
			buff_read[bufferIndex] = '\0';

			int n;
			sscanf( buff_read, "%d", &n );
			n++;
			sprintf( buff_write, "%d", n );
			buff_write[sizeof( buff_write ) - 1] = '\0';

			lseek( fileDescriptor, 0, SEEK_SET );

			write( fileDescriptor, buff_write,
			       strlen( buff_write ) );

			printf( "Enter to unlock this file.. \n" );
			getchar();
			lock.l_type = F_UNLCK;
			fcntl( fileDescriptor, F_SETLKW, &lock );
			printf( "The file is now unlocked. \n" );
		}
	}
	return 0;
}
