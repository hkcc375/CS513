/*
================================================================================================
Name : 29.c
Author : Hemant Kumar Chandavar
Description : Write a program to get scheduling policy and modify the scheduling
policy (SCHED_FIFO, SCHED_RR).
Date : 3rd September, 2023
================================================================================================
*/

#include <sched.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

struct sched_param p;

int main()
{
	p.sched_priority = 10;
	printf( "The scheduling policy of the current process is %d. \n",
	        sched_getscheduler( getpid() ) );
	printf( "Return Value is %d. \n",
	        sched_setscheduler( 0, SCHED_FIFO, &p ) );
	printf( "The scheduling policy of the current process is %d. \n",
	        sched_getscheduler( getpid() ) );
	printf( "Return Value is %d. \n",
	        sched_setscheduler( 0, SCHED_RR, &p ) );
	printf( "The scheduling policy of the current process is %d. \n",
	        sched_getscheduler( getpid() ) );
}
