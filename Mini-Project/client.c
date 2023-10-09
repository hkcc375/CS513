#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include "socket_constants.h"

#define BUFFER_SIZE 1024

int main( int argc, char const* argv[] )
{
	char message[BUFFER_SIZE];
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
			write( 1, INPUT_MESSAGE, sizeof( INPUT_MESSAGE ) );
			fgets( message, BUFFER_SIZE, stdin );
			if( !strcmp( message, "q\n" ) ||
			    !strcmp( message, "Q\n" ) )
				break;
			write( socketDescriptor, message, strlen( message ) );
			int str_len =
			    read( socketDescriptor, message, BUFFER_SIZE - 1 );
			message[str_len] = '\0';
			printf( "Message from the server : %s", message );
		}
		close( socketDescriptor );
	}
	return 0;
}
