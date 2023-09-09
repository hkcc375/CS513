/*
================================================================================================
Name : 8.c
Author : Hemant Kumar Chandavar
Description : Write a program to open a file in read only mode, read line by
line and display each line as it is read. Close the file when end of file is
reached.
Date : 28th August, 2023
================================================================================================
*/

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int main( int argc, char* argv[] )
{
	if( argc != 2 )
	{
		perror( "Pass the file name as command line argument" );
	}
	else
	{
		char* fileName     = argv[1];
		int fileDescriptor = open( fileName, O_RDONLY );
		if( fileDescriptor == -1 )
		{
			perror( "Error while opening the file. \n" );
		}
		else
		{
			char buffer[1];
			int bytesRead;
			while( ( bytesRead =
			             read( fileDescriptor, buffer, 1 ) ) == 1 )
			{
				write( 1, buffer, 1 );
			}
		}
		close( fileDescriptor );
	}
}