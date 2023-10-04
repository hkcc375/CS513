#include <assert.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main( int argc, char* argv[] )
{
	char *fileName_One, *fileName_Two;
	int fileDescriptor_One, fileDescriptor_Two;
	if( argc != 3 )
		printf( "Please pass 2 FIFO file names as command line "
		        "arguments. \n" );
	else
	{
		char bufferOne[1024], bufferTwo[1024];
		fileName_One = argv[1];
		fileName_Two = argv[2];

		fileDescriptor_One = open( fileName_One, O_WRONLY );
		printf(
		    "FIFO 1 file was successfully opened in WRITE mode. \n" );
		printf( "Message to write end of FIFO 1 : " );
		scanf( "%[^\n]", bufferOne );
		int i =
		    write( fileDescriptor_One, bufferOne, sizeof( bufferOne ) );
		assert( i >= 0 );

		fileDescriptor_Two = open( fileName_Two, O_RDONLY );
		printf(
		    "FIFO 2 file was successfully opened in READ mode. \n" );
		int j =
		    read( fileDescriptor_Two, bufferTwo, sizeof( bufferTwo ) );
		assert( j >= 0 );
		printf( "Message from the read end of FIFO 2 : %s. \n",
		        bufferTwo );
	}
	return 0;
}