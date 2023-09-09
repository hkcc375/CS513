/*
===============================================================================================
Name : 18a.c
Author : Hemant Kumar Chandavar
Description : Write a program to perform Record locking.
a. Implement write lock
b. Implement read lock
Create three records in a file. Whenever you access a particular record, first
lock it then modify/access to avoid race condition.
Date : 28th August, 2023
===============================================================================================
*/

#include "18.h"
#include <fcntl.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main()
{
	struct db ticket_db[3];
	for( int i = 0; i < 3; i++ )
	{
		ticket_db[i].train_num    = i + 1;
		ticket_db[i].ticket_count = 0;
	}
	int fileDescriptor =
	    open( "record.txt", O_CREAT | O_RDWR | O_TRUNC, 0777 );
	write( fileDescriptor, &ticket_db, sizeof( ticket_db ) );
}
