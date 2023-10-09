#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include "socket_constants.h"

int main( int argc, char const* argv[] )
{
	struct sockaddr_in serverAddress;

	if( argc != 3 )
	{
		write( 1, CLIENT_FILE_USAGE, sizeof( CLIENT_FILE_USAGE ) );
	}
	else
	{
		int socketDescriptor = socket( AF_INET, SOCK_STREAM, 0 );
		if( socketDescriptor == -1 )
		{
			write( 2, SOCKET_ERROR, sizeof( SOCKET_ERROR ) );
		}
		memset( &serverAddress, 0, sizeof( serverAddress ) );
		serverAddress.sin_family      = AF_INET;
		serverAddress.sin_port        = htons( atoi( argv[2] ) );
		serverAddress.sin_addr.s_addr = inet_addr( argv[1] );

		if( connect( socketDescriptor,
		             ( struct sockaddr* ) &serverAddress,
		             sizeof( serverAddress ) ) == -1 )
		{
			write( 2, CONNECT_ERROR, sizeof( CONNECT_ERROR ) );
		}
		else
		{
			write( 1, CLIENT_CONNECTED,
			       sizeof( CLIENT_CONNECTED ) );
		}

		while( 1 )
		{
			char *read_buffer, *write_buffer;
			read_buffer  = ( char* ) malloc( 512 * sizeof( char ) );
			write_buffer = ( char* ) malloc( 512 * sizeof( char ) );
			int str_len =
			    read( socketDescriptor, read_buffer, 512 );
			read_buffer[str_len] = '\0';
			printf( "%s\n", read_buffer );

			free( read_buffer );
			free( write_buffer );
		}
		close( socketDescriptor );
	}
	return 0;
}
