#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main( int argc, char const *argv[] )
{
	int fd[2];
	char buffer[128];
	int i;
	if( !( i = pipe( fd ) ) )
	{
		printf( "The pipe was created successfully. \n" );
		sprintf( buffer, "This is just a test message if both the ends "
		                 "of the pipe work correctly.\n" );
		int j = write( fd[1], buffer, sizeof( buffer ) );
		if( j < 0 )
			perror(
			    "There was an error while performing write.\n" );
		else
		{
			int k = read( fd[0], buffer, sizeof( buffer ) );
			assert( k >= 0 );
			printf( "Message from the read-end of the pipe "
			        ": %s",
			        buffer );
		}
	}
	else { perror( "There was an error while creating the pipe. \n" ); }
	return 0;
}