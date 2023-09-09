/*
===============================================================================================
Name : 18b.c
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
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int main( int argc, char* argv[] )
{
	char* fileName;
	int fileDescriptor;
	if( argc != 2 )
	{
		perror( "Pass the file name as a command line arguments. \n" );
	}
	else
	{
		printf( "The file is now open for write record locking. \n" );
		char* fileName     = argv[1];
		int fileDescriptor = open( fileName, O_RDWR );
		printf( "Select the train number : " );
		int train_record;
		scanf( "%d", &train_record );
		getchar();

		struct flock lock;
		lock.l_type   = F_WRLCK;
		lock.l_start  = ( train_record - 1 ) * sizeof( ticket_db );
		lock.l_whence = SEEK_SET;
		lock.l_len    = sizeof( ticket_db );
		lock.l_pid    = getpid();

		lseek( fileDescriptor,
		       ( train_record - 1 ) * sizeof( ticket_db ), SEEK_SET );
		read( fileDescriptor, &ticket_db, sizeof( ticket_db ) );

		fcntl( fileDescriptor, F_SETLKW, &lock );
		printf( "We are now going to lock record %d. \n",
		        train_record );

		printf( "The last ticket booked was : %d. \n",
		        ticket_db.ticket_count );
		printf( "To book the ticket, please press enter... \n" );
		ticket_db.ticket_count++;
		lseek( fileDescriptor, -1 * sizeof( ticket_db ), SEEK_CUR );
		write( fileDescriptor, &ticket_db, sizeof( ticket_db ) );
		getchar();
		lock.l_type = F_UNLCK;
		fcntl( fileDescriptor, F_SETLKW, &lock );
		printf( "The ticket is booked. \n" );
		printf( "The file is once again open for write record locking. "
		        "\n" );
	}

	return 0;
}
