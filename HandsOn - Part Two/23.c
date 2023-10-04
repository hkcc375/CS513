#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main( int argc, char const *argv[] )
{
	long PIPE_BUF, OPEN_MAX;
	PIPE_BUF = pathconf( ".", _PC_PIPE_BUF );
	OPEN_MAX = sysconf( _SC_OPEN_MAX );
	printf( "The maximum amount of data that can be written onto a "
	        "FIFO/pipe is %ld.\n",
	        PIPE_BUF );
	printf( "The maximum number of files that can be open within a process "
	        "is %ld.\n",
	        OPEN_MAX );
	return 0;
}