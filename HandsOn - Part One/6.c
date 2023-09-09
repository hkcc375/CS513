/*
================================================================================================
Name : 6.c
Author : Hemant Kumar Chandavar
Description : Write a program to take input from STDIN and display on STDOUT.
Use only read/write system calls
Date : 28th August, 2023
================================================================================================
*/

#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main( void )
{
	char buffer[100];
	while( 1 )
	{
		int read_bytes  = read( 0, buffer, strlen( buffer ) );
		int write_bytes = write( 1, buffer, strlen( buffer ) );
		if( read_bytes == -1 || write_bytes == -1 )
		{
			perror( "Error during read OR write. \n" );
			break;
		}
	}
}
