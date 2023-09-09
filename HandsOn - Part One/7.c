/*
================================================================================================
Name : 7.c
Author : Hemant Kumar Chandavar
Description : Write a program to copy file1 into file2 ($cp file1 file2).
Date : 28th August, 2023
================================================================================================
*/

#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int main( int argc, char* argv[] )
{
	if( argc != 3 )
	{
		printf( "Please pass appropriate file arguments for reading "
		        "and writing. \n" );
		return -1;
	}
	int fd_read  = open( argv[1], O_RDONLY, 0777 );
	int fd_write = open( argv[2], O_CREAT | O_WRONLY, 0777 );
	if( fd_write == -1 || fd_read == -1 )
		printf( "Read File or Write file was not opened properly. \n" );

	while( 1 )
	{
		char buf;
		int read_byte = read( fd_read, &buf, 1 );
		if( read_byte == 0 ) break;
		int write_byte = write( fd_write, &buf, 1 );
	}
	printf( "File copy has been successfully completed. \n" );
	int fd_close_read  = close( fd_read );
	int fd_close_write = close( fd_write );
	if( fd_close_read == -1 || fd_close_write == -1 )
		printf( "Error while closing file descriptors. \n" );
	return 0;
}
