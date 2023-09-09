/*
================================================================================================
Name : 4.c
Author : Hemant Kumar Chandavar
Description : Write a program to open an existing file with read write mode. Try
O_EXCL flag also.
Date : 28th August, 2023
================================================================================================
*/

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define RD_WR 0666

int main( int argc, char* argv[] )
{

	if( argc != 2 )
	{
		perror( "Please pass file name as command line argument. \n" );
	}
	else
	{
		char* fileName = argv[1];
		// Attempt to open the file with read-write mode and O_EXCL flag
		int fd = open( fileName, O_CREAT | O_RDWR | O_EXCL, RD_WR );

		if( fd == -1 )
		{
			if( errno == EEXIST )
			{
				// The file already exists
				printf(
				    "The file already exists. Opening it with "
				    "read-write mode.\n" );

				// Attempt to open the file without O_EXCL
				fd = open( fileName, O_RDWR, RD_WR );

				if( fd == -1 )
				{
					perror( "open" );
					exit( EXIT_FAILURE );
				}
			}
			else
			{
				// Some other error occurred
				perror( "open" );
				exit( EXIT_FAILURE );
			}
		}
		else
		{
			// The file was opened exclusively (O_EXCL), but it
			// didn't exist before
			printf( "The file did not exist, but it was created "
			        "and opened "
			        "exclusively.\n" );
		}
	}
	return 0;
}