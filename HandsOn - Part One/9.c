/*
================================================================================================
Name : 9.c
Author : Hemant Kumar Chandavar
Description : Write a program to print the following information about a given
file.
a. inode
b. number of hard links
c. uid
d. gid
e. size
f. block size
g. number of blocks
h. time of last access
i. time of last modification
j. time of last change
Date : 28th August, 2023
================================================================================================
*/

#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

#define RD_WR 0666

int main( int argc, char* argv[] )
{
	char* fileName;
	int fileDescriptor;
	if( argc != 2 )
		printf( "Pass the filename as the argument. \n" );
	else
	{
		fileName       = argv[1];
		fileDescriptor = creat( fileName, RD_WR );
		if( fileDescriptor == -1 )
			perror( "Error while creating the file. \n" );
		else
		{
			printf( "The file has been created successfully. \n" );
			struct stat s;
			stat( fileName, &s );
			printf( "%s - inode number : %ld \n", fileName,
			        s.st_ino );
			printf( "%s - number of hard links : %ld \n", fileName,
			        s.st_nlink );
			printf( "%s - uid : %ld \n", fileName, s.st_uid );
			printf( "%s - gid : %ld \n", fileName, s.st_gid );
			printf( "%s - size : %ld \n", fileName, s.st_size );
			printf( "%s - block size : %ld \n", fileName,
			        s.st_blksize );
			printf( "%s - number of blocks : %ld \n", fileName,
			        s.st_blocks );
			printf( "%s - time of last access : %s", fileName,
			        ctime( &s.st_atim.tv_sec ) );
			printf( "%s - time of last modification : %s", fileName,
			        ctime( &s.st_mtim.tv_sec ) );
			printf( "%s - time of last change : %s", fileName,
			        ctime( &s.st_ctim.tv_sec ) );
		}
	}
}
