/*
================================================================================================
Name : 23.c
Author : Hemant Kumar Chandavar
Description : Write a program to create a Zombie state of the running program.
Date : 3rd September, 2023
================================================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

int main( int argc, char* argv[] )
{
	pid_t pID = fork();
	if( pID > 0 )
	{
		printf( "I am the parent process and my PID is %d. I am now "
		        "going to sleep for 120 sec. \n",
		        getpid() );
		sleep( 120 );
	}
	else
	{
		printf( "I am the child process and my PID is %d. I am now "
		        "going to exit normally. \n",
		        getpid() );
		exit( 0 );
	}
}
