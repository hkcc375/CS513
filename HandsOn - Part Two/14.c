#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main( int argc, char const *argv[] )
{
	int fd[2];
	char buffer[128];
	int retVal = pipe( fd );
	if( !retVal )
	{
		printf( "The pipe was created successfully. \n" );
		sprintf( buffer, "Test Message - Works Correctly.\n" );
		int i = write( fd[1], buffer, sizeof( buffer ) );
		assert( i >= 0 );
		int j = read( fd[0], buffer, sizeof( buffer ) );
		assert( j >= 0 );
		printf( "Message from the read-end of the pipe "
		        ": %s",
		        buffer );
	}
	else { perror( "There was an error while creating the pipe. \n" ); }
	return 0;
}