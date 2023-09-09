/*
=================================================================================================
Name : 12.c
Author : Hemant Kumar Chandavar
Description : Write a program to find out the opening mode of a file. Use fcntl.
Date : 28th August, 2023
=================================================================================================
*/

#include <fcntl.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main( int argc, char* argv[] )
{
	if( argc != 2 )
	{
		perror( "Pass the file name as command line argument. \n" );
	}
	else
	{
		char* fileName     = argv[1];
		int fileDescriptor = open( fileName, O_CREAT | O_RDWR, 0777 );
		if( fileDescriptor < 0 )
		{
			perror( "Error while opening the file. \n" );
		}
		else
		{
			int flags = fcntl( fileDescriptor, F_GETFL );
			switch( flags & O_ACCMODE )
			{
			case O_RDONLY:
				printf(
				    "File %s is opened in read-only mode. \n",
				    fileName );
				break;
			case O_WRONLY:
				printf(
				    "File %s is opened in write-only mode. \n",
				    fileName );
				break;
			case O_RDWR:
				printf(
				    "File %s is opened in read-write mode. \n",
				    fileName );
				break;
			default:
				printf( "Can't identify the opening mode of "
				        "the file %s. \n",
				        fileName );
				break;
			}
		}
	}
	return 0;
}
