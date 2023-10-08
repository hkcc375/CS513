#include <arpa/inet.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <unistd.h>

#define BUFFER_SIZE 30

void error_handling( char* message );

void error_handling( char* message )
{
	fputs( message, stderr );
	fputc( '\n', stderr );
	exit( 1 );
}

int main( int argc, char const* argv[] )
{
	int serverSocket, clientSocket;
	struct sockaddr_in serverAddress, clientAddress;
	struct sigaction act;
	socklen_t addressSize;
	char buffer[BUFFER_SIZE];

	if( argc != 2 )
		printf( "Usage : %s <port-no>", argv[0] );
	else
	{
		serverSocket = socket( AF_INET, SOCK_STREAM, 0 );
		if( serverSocket == -1 ) error_handling( "socket() error. \n" );
		memset( &serverAddress, 0, sizeof( serverAddress ) );
		serverAddress.sin_family      = AF_INET;
		serverAddress.sin_addr.s_addr = htonl( INADDR_ANY );
		serverAddress.sin_port        = htons( atoi( argv[1] ) );

		if( bind( serverSocket, ( struct sockaddr* ) &serverAddress,
		          sizeof( serverAddress ) ) == -1 )
			error_handling( "bind() error. \n" );

		if( listen( serverSocket, 5 ) == -1 )
			error_handling( "listen() error. \n" );

		addressSize = sizeof( clientAddress );
		clientSocket =
		    accept( serverSocket, ( struct sockaddr* ) &clientAddress,
		            &addressSize );
		if( clientSocket == -1 )
			error_handling( "accept() error. \n" );
		else
			puts( "New client has connected to the "
			      "server." );
		int str_len;
		while( ( str_len =
		             read( clientSocket, buffer, BUFFER_SIZE ) ) != 0 )
			write( clientSocket, buffer, str_len );
		close( clientSocket );
		close( serverSocket );
		puts( "Client disconnected." );
	}
	return 0;
}
