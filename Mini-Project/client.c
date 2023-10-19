#include <arpa/inet.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include "socket_constants.h"

void* send_message( void* arg );
void* receive_message( void* arg );

void* send_message( void* arg )
{
	int socket = *( ( int* ) arg );
	char* read_buffer;
	read_buffer = ( char* ) malloc( 512 * sizeof( char ) );
	while( 1 )
	{
		memset( read_buffer, 0, 512 );
		read( 0, read_buffer, 512 );
		int str_len = write( socket, read_buffer, strlen( read_buffer ) );
		if( str_len == -1 ) return ( void* ) -1;
	}
	return NULL;
}

void* receive_message( void* arg )
{
	int socket = *( ( int* ) arg );
	char* write_buffer;
	write_buffer = ( char* ) malloc( 512 * sizeof( char ) );
	int str_len;
	while( 1 )
	{
		memset( write_buffer, 0, 512 );
		sleep( 1 );
		str_len = read( socket, write_buffer, 512 );
		if( str_len == -1 )
		{
			write( 1, CLIENT_SOCKET_CLOSED, sizeof( CLIENT_SOCKET_CLOSED ) );
			return ( void* ) -1;
		}
		write( 1, write_buffer, strlen( write_buffer ) );
	}
	return NULL;
}

int main( int argc, char const* argv[] )
{
	struct sockaddr_in serverAddress;
	void* thread_return;

	if( argc != 3 ) { write( 1, CLIENT_FILE_USAGE, sizeof( CLIENT_FILE_USAGE ) ); }
	else
	{
		int socketDescriptor = socket( AF_INET, SOCK_STREAM, 0 );
		if( socketDescriptor == -1 ) { write( 2, SOCKET_ERROR, sizeof( SOCKET_ERROR ) ); }
		memset( &serverAddress, 0, sizeof( serverAddress ) );
		serverAddress.sin_family      = AF_INET;
		serverAddress.sin_port        = htons( atoi( argv[2] ) );
		serverAddress.sin_addr.s_addr = inet_addr( argv[1] );

		if( connect( socketDescriptor, ( struct sockaddr* ) &serverAddress, sizeof( serverAddress ) ) == -1 )
		{
			write( 2, CONNECT_ERROR, sizeof( CONNECT_ERROR ) );
		}
		else
		{
			write( 1, CLIENT_CONNECTED, sizeof( CLIENT_CONNECTED ) );
			pthread_t send_thread, receive_thread;
			pthread_create( &send_thread, NULL, send_message, ( void* ) &socketDescriptor );
			pthread_create( &receive_thread, NULL, receive_message, ( void* ) &socketDescriptor );
			pthread_join( send_thread, &thread_return );
			pthread_join( receive_thread, &thread_return );
		}
		close( socketDescriptor );
	}
	return 0;
}
