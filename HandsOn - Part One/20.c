/*
================================================================================================
Name : 20.c
Author : Hemant Kumar Chandavar
Description : Find out the priority of your running program. Modify the priority
with nice command.
Date : 3rd September, 2023
================================================================================================
*/

#include <stdio.h>
#include <sys/resource.h>
#include <unistd.h>

int main( int argc, char* argv[] )
{
	printf( "The current process has the PID : %d. \n", getpid() );
	printf( "The priority of the current process is = %d \n",
	        getpriority( PRIO_PROCESS, 0 ) );
	getchar();
	printf( "I am now going to change the nice value. \n" );
	nice( 4 );
	printf( "The modified priority is = %d \n",
	        getpriority( PRIO_PROCESS, 0 ) );
	getchar();
}
