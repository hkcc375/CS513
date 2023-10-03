// Using fcntl

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main( int argc, char const *argv[] )
{
	int fd[2];
	int retVal = pipe( fd );
	if( !retVal )
	{
		printf( "The pipe was created successfully. \n" );
		if( !fork() )
		{
			close( 1 );
			close( fd[0] ); // Close the read end of the pipe.
			fcntl( fd[1], F_DUPFD, 0 );
			execlp( "ls", "ls", "-l", NULL );
		}
		else
		{
			close( 0 );
			close( fd[1] ); // Close the write end of the pipe.
			fcntl( fd[0], F_DUPFD, 0 );
			execlp( "wc", "wc", NULL );
		}
	}
	else
		perror( "There was an error while creating the pipe. \n" );
	return 0;
}