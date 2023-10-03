#include <assert.h>
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
		char buffer[128];
		if( !fork() )
		{
			close( fd[1] );
			int j = read( fd[0], buffer, sizeof( buffer ) );
			assert( j >= 0 );
			printf( "Message from the parent : %s", buffer );
		}
		else
		{
			close( fd[0] );
			sprintf( buffer, "Hi, This message is from papa.\n" );
			buffer[strlen( buffer )] = '\0';
			int i = write( fd[1], buffer, sizeof( buffer ) );
			assert( i >= 0 );
		}
	}
	else
		perror( "There was an error while creating the pipe. \n" );
	return 0;
}