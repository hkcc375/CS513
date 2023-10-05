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
		fd_set readfds;
		struct timeval tv;

		fileDescriptor = open( fileName, O_RDONLY );
		if( fileDescriptor < 0 ) perror( "Error : " );
		printf( "File Descriptor value is : %d.\n", fileDescriptor );
		int nfds = fileDescriptor + 1;

		tv.tv_sec  = 10;
		tv.tv_usec = 0;

		FD_ZERO( &readfds );
		FD_SET( fileDescriptor, &readfds );

		int retval = select( nfds, &readfds, NULL, NULL, &tv );
		if( retval == 0 )
			printf( "Data was not read from FIFO within 10 "
			        "seconds. \n" );
		else if( retval == -1 )
			perror( "Error : Either we've used a bad file "
			        "descriptor OR an "
			        "interrupt occured. \n" );
		else
		{
			int i =
			    read( fileDescriptor, buffer, sizeof( buffer ) );
			assert( i >= 0 );
			printf( "Data was read from FIFO file. \n" );
			printf( "Output on the read end of FIFO : %s. \n",
			        buffer );
			close( fileDescriptor );
		}
	}
	return 0;
}
