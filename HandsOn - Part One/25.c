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
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main()
{
	pid_t childAPID, childBPID, childCPID, terminatedChildPID;
	int waitStatus;

	if( ( childAPID = fork() ) == 0 )
	{
		printf( "Child A created...\n" );
		printf( "Putting child A is now going to sleep.\n" );
		sleep( 5 );
		printf( "Child A is now awake!\n" );
		exit( 0 );
	}
	else
	{
		if( ( childBPID = fork() ) == 0 )
		{
			// Child B will enter this branch
			printf( "Child B created...\n" );
			printf( "Putting child B is now going to sleep.\n" );
			sleep( 10 );
			printf( "Child B is now awake!\n" );
			exit( 0 );
		}
		else
		{

			if( ( childCPID = fork() ) == 0 )
			{
				printf( "Child C created...\n" );
				printf( "Putting child C is now going to "
				        "sleep.\n" );
				sleep( 15 );
				printf( "Child C is now awake!\n" );
				exit( 0 );
			}
			else
			{
				terminatedChildPID =
				    waitpid( childCPID, &waitStatus, 0 );

				if( terminatedChildPID == childCPID )
				{
					if( waitStatus == 0 )
						printf(
						    "Child C has terminated "
						    "with exit status = 0\n" );
					else
						printf( "Child has exited with "
						        "some other "
						        "exit status : %d\n",
						        waitStatus );
				}
			}
		}
	}
}
