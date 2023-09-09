/*
================================================================================================
Name : 27d.c
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
	char* arguments[4];
	arguments[0] = "ls";
	arguments[1] = "-R";
	arguments[2] = "-l";
	arguments[3] = NULL;
	execv( "/bin/ls", arguments );
}
