#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "admin.h"
#include "server_constants.h"

#define ADMIN_ID "admin"
#define ADMIN_PASSWORD "1234"

void admin_handler( int clientSocket )
{
	char *read_buffer, *write_buffer, *username_buffer, *password_buffer;
	read_buffer     = ( char* ) malloc( 512 * sizeof( char ) );
	write_buffer    = ( char* ) malloc( 512 * sizeof( char ) );
	username_buffer = ( char* ) malloc( 16 * sizeof( char ) );
	password_buffer = ( char* ) malloc( 16 * sizeof( char ) );
	while( 1 )
	{
		memset( read_buffer, 0, 512 );
		memset( write_buffer, 0, 512 );
		memset( username_buffer, 0, 16 );
		memset( password_buffer, 0, 16 );
		write( clientSocket, LOGIN_MESSAGE, sizeof( LOGIN_MESSAGE ) );
		int read_username_bytes =
		    read( clientSocket, username_buffer, 16 );

		username_buffer[read_username_bytes - 1] = '\0';
		write( 1, username_buffer, strlen( username_buffer ) );
		write( clientSocket, PASSWORD_MESSAGE,
		       sizeof( PASSWORD_MESSAGE ) );
		int read_password_bytes =
		    read( clientSocket, password_buffer, 16 );

		write( 1, password_buffer, strlen( password_buffer ) );
		password_buffer[read_password_bytes - 1] = '\0';

		if( !strcmp( ADMIN_ID, username_buffer ) &&
		    !( strcmp( ADMIN_PASSWORD, password_buffer ) ) )
		{
			write( clientSocket, AUTHENTICATION_SUCCESS,
			       sizeof( AUTHENTICATION_SUCCESS ) );
			write( clientSocket, ADMIN_MENU, sizeof( ADMIN_MENU ) );
			break;
		}
		else
			write( clientSocket, AUTHENTICATION_FAILED,
			       sizeof( AUTHENTICATION_FAILED ) );
	}
}