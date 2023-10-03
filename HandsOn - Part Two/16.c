#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main( int argc, char const *argv[] )
{
	int fd1[2], fd2[2];
	int retVal_One = pipe( fd1 );
	int retVal_Two = pipe( fd2 );
	if( !retVal_One && !retVal_Two )
	{
		printf( "Both the pipes were created successfully. \n" );
		char buffer[128];
		if( !fork() ) // Child
		{
			char buff[64];

			close( fd1[0] );
			close( fd2[1] );
			printf( "In the Child Process - enter message : " );
			scanf( "%[^\n]", buff );
			int i = write( fd1[1], buff, sizeof( buff ) );
			assert( i >= 0 );
			int j = read( fd2[0], buffer, sizeof( buffer ) );
			assert( j >= 0 );
			printf(
			    "In the Child Process - message from parent : %s",
			    buffer );
		}
		else // Parent
		{
			char buff[64];

			close( fd1[1] );
			close( fd2[0] );
			int i = read( fd1[0], buff, sizeof( buff ) );
			assert( i >= 0 );
			printf(
			    "In the Parent Process - message from child : %s\n",
			    buff );
			printf( "In the Parent Process - enter message : " );
			scanf( "%[^\n]", buffer );
			int j = write( fd2[1], buffer, sizeof( buffer ) );
			assert( j >= 0 );
		}
	}
	else
		perror( "There was an error while creating pipes. \n" );
	return 0;
}