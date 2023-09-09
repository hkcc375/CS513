/*
================================================================================================
Name : 10.c
Author : Hemant Kumar Chandavar
Description : Write a program to open a file with read write mode, write 10
bytes, move the file pointer by 10 bytes (use lseek) and write again 10 bytes.
a. check the return value of lseek
b. open the file with od and check the empty spaces in between the data.
Date : 28th August, 2023
================================================================================================
*/

#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main()
{
	char text_content[] = "Hello.....";
	int fd              = open( "seekfile.c", O_CREAT | O_RDWR, 0744 );
	if( fd == -1 )
		perror( "Error opening the file. \n" );
	else
	{
		write( fd, text_content, strlen( text_content ) );
		int offset = lseek( fd, 10, SEEK_CUR );
		if( offset == -1 )
			perror( "Error while using lseek. \n" );
		else
		{
			write( fd, text_content, strlen( text_content ) );
			printf( "The resulting offset location is %d. \n",
			        offset );
		}
	}
}
