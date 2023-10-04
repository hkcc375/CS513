#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

#define RD_WR 0666

int main( int argc, char* argv[] )
{
	char* fileName;
	int retval;
	if( argc != 2 )
	{
		printf( "Please pass the file name as the command line "
		        "argument.\n" );
	}
	else
	{
		fileName = argv[1];
		retval   = mknod( fileName, S_IFIFO | RD_WR, 0 );
		if( retval == -1 )
			perror( "Error while creating the FIFO file. \n" );
		else
			printf( "The FIFO file was created successfully. \n" );
	}
	return 0;
}