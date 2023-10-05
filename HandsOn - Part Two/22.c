#include <assert.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/select.h>
#include <sys/time.h>
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
		fileName = argv[1];
		fd_set writefds;
		struct timeval tv;

		fileDescriptor = open( fileName, O_NONBLOCK | O_WRONLY );
		if( fileDescriptor < 0 ) perror( "Error : " );
		printf( "File Descriptor value is : %d.\n", fileDescriptor );
		int nfds = fileDescriptor + 1;

		tv.tv_sec  = 10;
		tv.tv_usec = 0;

		FD_ZERO( &writefds );
		FD_SET( fileDescriptor, &writefds );

		scanf( "%[^\n]", buffer );
		int i = write( fileDescriptor, buffer, sizeof( buffer ) );
		assert( i >= 0 );

		int retval = select( nfds, NULL, &writefds, NULL, &tv );
		if( retval == 0 )
			printf( "Data was not written into FIFO within 10 "
			        "seconds. \n" );
		else if( retval == -1 )
			perror( "Error : Either we've used a bad file "
			        "descriptor OR an "
			        "interrupt occured. \n" );
		else
			printf( "Data was written into FIFO within 10 seconds. "
			        "\n" );
	}
	return 0;
}
