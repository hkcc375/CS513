/*
================================================================================================
Name : 27a.c
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

int main() { execl( "/bin/ls", "ls", "-R", "-l", NULL ); }
