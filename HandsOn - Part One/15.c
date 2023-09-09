/*
================================================================================================
Name : 15.c
Author : Hemant Kumar Chandavar
Description : Write a program to display the environmental variable of the user
(use environ).
Date : 28th August, 2023
================================================================================================
*/

#include <stdio.h>
#include <stdlib.h>

extern char** environ;

int main()
{
	char** env = environ;
	while( *env != NULL )
	{
		printf( "%s \n", *env );
		env++;
	}
	return 0;
}
