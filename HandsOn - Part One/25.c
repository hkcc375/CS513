/*
================================================================================================
Name : 25.c
Author : Hemant Kumar Chandavar
Description : Write a program to create three child processes. The parent should
wait for a particular child (use waitpid system call).
Date : 3rd September,2023
================================================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

int main( int argc, char* argv[] )
{
	pid_t pid_childOne = fork();
	pid_t pid_childTwo = fork();
	int status;
	if( pid_childOne != 0 && pid_childTwo != 0 )
	{
		printf( "Hi, I am the parent of all the child processes, and "
		        "my PID is %d. \n",
		        getpid() );
		printf( "I am now going to wait for the Child Process 1, and "
		        "its PID is %d. \n",
		        pid_childOne );

		while( !waitpid( pid_childOne, &status, 0 ) )
			;

		printf( "The Child Process 1 has successully terminated and "
		        "now I am resuming execution. \n" );
		printf( "The exit status code of Child Process 1 is %d. \n",
		        WEXITSTATUS( status ) );
	}
	else if( pid_childOne == 0 && pid_childTwo != 0 )
	{
		printf( "Hi, I am Child Process 1 (in the order of creation), "
		        "and my PID is %d. \n",
		        getpid() );
		printf( "Also, I have created Child Process 3 and its PID is "
		        "%d. \n",
		        pid_childTwo );
		printf( "I am now going to sleep. \n" );
		sleep( 20 );
		printf( "I am now going to exit with status code 0. \n" );
		exit( 0 );
	}
	else if( pid_childOne != 0 && pid_childTwo == 0 )
	{
		printf( "Hi, I am Child Process 2 (in the order of creation), "
		        "and my PID is %d. \n",
		        getpid() );
		printf( "I am now going to exit with status code 1. \n" );
		exit( 1 );
	}
	else
	{
		printf( "Hi, I am Child Process 3 (in the order of creation), "
		        "and my PID is %d. \n",
		        getpid() );
		printf( "I am now going to exit with status code 2. \n" );
		exit( 2 );
	}
	return 0;
}
