#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main( int argc, char const *argv[] )
{
	int fd1[2], fd2[2];
	int retVal_One = pipe( fd1 );
	int retVal_Two = pipe( fd2 );
	if( !retVal_One && !retVal_Two )
	{
		if( !fork() ) // Child 1
		{
			close( fd1[0] ); // Close the read end of pipe 1.
			close( fd2[0] ); // Close both ends of pipe 2 because ls
			                 // -Rl is not using it.
			close( fd2[1] );
			dup2( fd1[1], 1 );
			execlp( "ls", "ls", "-l", NULL );
		}
		else
		{
			if( !fork() ) // Child 2
			{
				close(
				    fd1[1] ); // Close the write end of pipe 1.
				close(
				    fd2[0] ); // Close the read end of pipe 2.
				dup2( fd1[0], 0 );
				dup2( fd2[1], 1 );
				execlp( "grep", "grep", "^d", NULL );
			}
			else // Parent
			{
				close( fd1[0] ); // Close both ends of pipe 1
				                 // because wc is not using it.
				close( fd1[1] );
				close(
				    fd2[1] ); // Close the write end of pipe 2.
				dup2( fd2[0], 0 );
				execlp( "wc", "wc", NULL );
			}
		}
	}
	else
		perror( "There was an error while creating the pipes. \n" );
	return 0;
}