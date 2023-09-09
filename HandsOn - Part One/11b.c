/*
================================================================================================
Name : 11b.c
Author : Hemant Kumar Chandavar
Description : Write a program to open a file, duplicate the file descriptor and
append the file with both the descriptors and check whether the file is updated
properly or not.
a. use dup
b. use dup2
c. use fcntl
Date : 28th August, 2023
================================================================================================
*/

#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main( void )
{
	int fd1, fd2;
	fd1 = open( "duplicate_file.c", O_CREAT | O_RDWR, 0777 );
	if( fd1 == -1 )
		perror( "Error while opening the file. \n" );
	else
	{
		char text_content[] =
		    "This is the content that I want to write. \n";
		write( fd1, text_content, strlen( text_content ) );
		int fd2 = dup2( fd1, 7 );
		if( fd2 == -1 )
			perror( "There is an error while duplicating the file "
			        "descriptor. \n" );
		else
		{
			printf( "The file descriptor values are : fd1 -> %d, "
			        "fd2 -> %d. \n",
			        fd1, fd2 );
			write( fd2, text_content, strlen( text_content ) );
			getchar();
			close( fd1 );
			close( fd2 );
		}
	}
}
