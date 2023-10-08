#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define BUFFER_SIZE 1024

void error_handling( char* message );

void error_handling( char* message )
{
	fputs( message, stderr );
	fputc( '\n', stderr );
	exit( 1 );
}

int main( int argc, char const* argv[] )
{
	char message[BUFFER_SIZE];
	struct sockaddr_in serverAddress;

	if( argc != 3 )
	{
		printf( "Usage : %s <ip-address> <port-no> . \n", argv[0] );
	}
	else
	{
		int socketDescriptor = socket( AF_INET, SOCK_STREAM, 0 );
		if( socketDescriptor == -1 )
			error_handling( "socket() error. \n" );
		memset( &serverAddress, 0, sizeof( serverAddress ) );
		serverAddress.sin_family      = AF_INET;
		serverAddress.sin_port        = htons( atoi( argv[2] ) );
		serverAddress.sin_addr.s_addr = inet_addr( argv[1] );

		if( connect( socketDescriptor,
		             ( struct sockaddr* ) &serverAddress,
		             sizeof( serverAddress ) ) == -1 )
			error_handling( "connect() error. \n" );
		else
			puts( "\nI am now connected...\n" );

		while( 1 )
		{
			fputs( "Input message (q or Q to exit) : ", stdout );
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
