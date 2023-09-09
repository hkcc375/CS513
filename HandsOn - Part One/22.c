/*
================================================================================================
Name : 22.c
Author : Hemant Kumar Chandavar
Description : Write a program, open a file, call fork, and then write to the
file by both the child as well as the parent processes. Check output of the
file.
Date : 3rd September, 2023
================================================================================================
*/

#include <fcntl.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main( int argc, char* argv[] )
{
	if( argc != 2 )
	{
		perror( "Please pass the name of the file that we want to "
		        "open. \n" );
	}
	else
	{
		char* fileName     = argv[1];
		int fileDescriptor = open( fileName, O_CREAT | O_RDWR, 0777 );
		if( fileDescriptor == -1 )
		{
			perror( "There was a problem in opening the file. \n" );
		}
		else
		{
			if( !fork() )
			{
				char buffer_child[] =
				    "I'm the child, and I am going to write to "
				    "the file.";
				write( fileDescriptor, buffer_child,
				       sizeof( buffer_child ) );
				printf( "Child has finished writing to the "
				        "file. \n" );
			}
			else
			{
				char buffer_parent[] =
				    "I'm the parent, and I am going to write "
				    "to the file.";
				write( fileDescriptor, buffer_parent,
				       sizeof( buffer_parent ) );
				printf( "Parent has finished writing to the "
				        "file. \n" );
			}
		}
		close( fileDescriptor );
	}
}
