/*
================================================================================================
Name : 27e.c
Author : Hemant Kumar Chandavar
Description : Write a program to execute ls -Rl by the following system calls
a.execl
b.execlp
c.execle
d.execv
e.execvp
Date : 3rd September, 2023
================================================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
	char* argument_list[4];
	argument_list[0] = "ls";
	argument_list[1] = "-R";
	argument_list[2] = "-l";
	argument_list[3] = NULL;
	execvp( "ls", argument_list );
}
