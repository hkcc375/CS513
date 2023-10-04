#include <assert.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main( int argc, char* argv[] )
{
	char* fileName;
	int fileDescriptor;
	if( argc != 2 )
		printf( "Please pass any FIFO file name as a command line "
		        "argument. \n" );
	else
	{
		char buffer[1024];
		fileName       = argv[1];
		fileDescriptor = open( fileName, O_RDONLY );
		printf( "FIFO file was successfully opened in READ mode. \n" );
		int i = read( fileDescriptor, buffer, sizeof( buffer ) );
		assert( i >= 0 );
		printf( "Message from the read end of FIFO : %s. \n", buffer );
	}
	return 0;
}