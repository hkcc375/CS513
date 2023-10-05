#include <assert.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main( int argc, char* argv[] )
{
	char* fileName;
	int fileDescriptor;
	char buffer[1024];

	if( argc != 2 )
		printf(
		    "Please pass FIFO file name as command line argument. \n" );
	else
	{
		fileName       = argv[1];
		fileDescriptor = open( fileName, O_WRONLY );

		scanf( "%[^\n]", buffer );
		int i = write( fileDescriptor, buffer, strlen( buffer ) );
		assert( i >= 0 );
		close( fileDescriptor );
	}
	return 0;
}
