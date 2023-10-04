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
		fileDescriptor = open( fileName, O_WRONLY );
		printf( "FIFO file was successfully opened in WRITE mode. \n" );
		printf( "Message to write end of FIFO : " );
		scanf( "%[^\n]", buffer );
		int i = write( fileDescriptor, buffer, sizeof( buffer ) );
		assert( i >= 0 );
	}
	return 0;
}