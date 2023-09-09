/*
================================================================================================
Name : 24.c
Author : Hemant Kumar Chandavar
Description : Write a program to create an orphan process.
Date : 3rd September, 2023
================================================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

int main( int argc, char* argv[] )
{
	pid_t pid = fork();
	if( pid > 0 )
	{
		printf( "I am the parent process and my PID is %d. I am now "
		        "going to terminate without waiting for my child. \n",
		        getpid() );
		exit( 0 );
	}
	else
	{
		printf( "I am the child process and my PID is %d. I am now "
		        "going to sleep for 120 seconds. \n",
		        getpid() );
		sleep( 120 );
	}
}
