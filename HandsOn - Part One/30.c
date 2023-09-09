/*
================================================================================================
Name : 30.c
Author : Hemant Kumar Chandavar
Description : Write a program to run a script at a specific time using a Daemon
process.
Date : 3rd September, 2023
================================================================================================
*/

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>

void executeScript() { 
  execl("/bin/ls", "ls", "-l", NULL);
}

int main( int argc, char* argv[] )
{
        time_t currentEpoch, deadlineEpoch; // Current system time & deadline time in epoch
        struct tm *deadline;                // Deadline in user readable format
	if( argc != 4 )
	{
		perror( "Pass the file name as a command line argument. \n" );
	}
	 else
	 {
	        time(&currentEpoch); // Get current time
                deadline = localtime(&currentEpoch);
                deadline->tm_hour = atoi(argv[1]);
                deadline->tm_min = argv[2] == NULL ? 0 : atoi(argv[2]);
                deadline->tm_sec = argv[3] == NULL ? 0 : atoi(argv[3]);
                deadlineEpoch = mktime(deadline); // Convert dealine to epoc
	        if( !fork() )
	        {
		        printf( "Child PID is %d. \n", getpid() );
		        setsid();
		        chdir( "/" );
		        umask( 0 );
		        printf("I have reached here. \n");
		        do
                        {
                                time(&currentEpoch);
                        } 
                        while (difftime(deadlineEpoch, currentEpoch) > 0);
                        executeScript();
                        exit(0);
	        }
	        else { exit( 0 ); }
	}
	return 0;
}
