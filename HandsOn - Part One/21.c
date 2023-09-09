/*
================================================================================================
Name : 21.c
Author : Hemant Kumar Chandavar
Description : Write a program, call fork and print the parent and child process
id.
Date : 3rd September, 2023
================================================================================================
*/

#include <stdio.h>
#include <unistd.h>

int main( int argc, char* argv[] )
{
	int childPID = fork();
	if( childPID )
	{
		printf( "I'm the parent and the process ID of my child process "
		        "is : %d. \n",
		        childPID );
		printf( "My process ID is : %d. \n", getpid() );
	}
	else
	{
		printf( "I'm the child and my process ID is : %d. \n",
		        getpid() );
		printf( "My parent's process ID is : %d. \n", getppid() );
	}
}
