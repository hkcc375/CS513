/*
================================================================================================
Name : 14.c
Author : Hemant Kumar Chandavar
Description : Write a program to find the type of a file.
a. Input should be taken from command line.
b. program should be able to identify any type of a file.
Date : 28th August, 2023
================================================================================================
*/

#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int main( int argc, char* argv[] )
{
	char* fileName;
	int fileDescriptor;
	if( argc != 2 )
		perror( "Pass the filename as the argument. \n" );
	else
	{
		fileName = argv[1];
		struct stat s;
		stat( fileName, &s );
		switch( s.st_mode & S_IFMT )
		{
		case S_IFBLK:
			printf( "%s is a block device. \n", fileName );
			break;
		case S_IFREG:
			printf( "%s is a regular file. \n", fileName );
			break;
		case S_IFLNK:
			printf( "%s is a symlink file. \n", fileName );
			break;
		case S_IFCHR:
			printf( "%s is a character device. \n", fileName );
			break;
		case S_IFDIR:
			printf( "%s is a directory. \n", fileName );
			break;
		case S_IFIFO:
			printf( "%s is a FIFO file. \n", fileName );
			break;
		case S_IFSOCK:
			printf( "%s is a socket. \n", fileName );
			break;
		default:
			printf( "%s has an unknown file type. \n", fileName );
			break;
		}
	}
}
