/*
================================================================================================
Name : 5.c
Author : Hemant Kumar Chandavar
Description : Write a program to create five new files with infinite loop.
Execute the program in the background and check the file descriptor table at
/proc/pid/fd.
Date : 28th August, 2023
================================================================================================
*/

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main( void )
{
	int fd1, fd2, fd3, fd4, fd5;
	fd1 = open( "file1.c", O_CREAT | O_RDWR );
	fd2 = open( "file2.c", O_CREAT | O_RDWR );
	fd3 = open( "file3.c", O_CREAT | O_RDWR );
	fd4 = open( "file4.c", O_CREAT | O_RDWR );
	fd5 = open( "file5.c", O_CREAT | O_RDWR );
	while( 1 )
		;
}
