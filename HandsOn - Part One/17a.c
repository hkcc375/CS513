/*
=============================================================================================
Name : 17a.c
Author : Hemant Kumar Chandavar
Description : Write a program to simulate online ticket reservation. Implement
write lock Write a program to open a file, store a ticket number and exit. Write
a separate program, to open the file, implement write lock, read the ticket
number, increment the number and print the new ticket number then close the
file.
Date : 28th August, 2023
=============================================================================================
*/

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
int main( int argc, char* argv[] )
{
	if( argc != 2 )
	{
		perror( "Please pass file as a command line argument. \n" );
	}
	else
	{

		char* fileName = argv[1];
		int fileDescriptor =
		    open( fileName, O_CREAT | O_RDWR | O_TRUNC, 0777 );
		if( fileDescriptor == -1 )
		{
			perror( "Error while opening a file. \n" );
		}
		else
		{
			int ticketNumber;
			printf( "Enter the ticket number : " );
			scanf( "%d", &ticketNumber );
			char buff[10];
			sprintf( buff, "%d", ticketNumber );
			buff[sizeof( buff ) - 1] = '\0';
			write( fileDescriptor, buff, strlen( buff ) );
			close( fileDescriptor );
		}
	}
	return 0;
}
