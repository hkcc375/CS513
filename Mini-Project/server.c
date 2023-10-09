#include <arpa/inet.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <unistd.h>

#include "socket_constants.h"

#define BUFFER_SIZE 30

void reap_child_process( int signal );

void reap_child_process( int signal )
{
	int status;
	pid_t childPid = waitpid( -1, &status, WNOHANG );
	if( childPid == -1 )
	{
		write( 2, WAITPID_UNSUCCESSFUL,
		       sizeof( WAITPID_UNSUCCESSFUL ) );
	}
	else if( childPid == 0 )
	{
		write( 1, WAITPID_STATUSNOTAVAILABLE,
		       sizeof( WAITPID_STATUSNOTAVAILABLE ) );
	}
	else
	{
		write( 1, PROCESS_REAPED_SUCCESS,
		       sizeof( PROCESS_REAPED_SUCCESS ) );
		printf( "Server removed the forked child process : %d. \n",
		        childPid );
	}
}

int main( int argc, char const* argv[] )
{
	int serverSocket, clientSocket;
	struct sockaddr_in serverAddress, clientAddress;
	struct sigaction act;
	socklen_t addressSize;
	char buffer[BUFFER_SIZE];

	if( argc != 2 )
	{
		write( 1, SERVER_FILE_USAGE, sizeof( SERVER_FILE_USAGE ) );
	}
	else
	{
		signal( SIGCHLD, reap_child_process );

		serverSocket = socket( AF_INET, SOCK_STREAM, 0 );
		if( serverSocket == -1 )
		{
			write( 2, SOCKET_ERROR, sizeof( SOCKET_ERROR ) );
		}
		memset( &serverAddress, 0, sizeof( serverAddress ) );
		serverAddress.sin_family      = AF_INET;
		serverAddress.sin_addr.s_addr = htonl( INADDR_ANY );
		serverAddress.sin_port        = htons( atoi( argv[1] ) );

		if( bind( serverSocket, ( struct sockaddr* ) &serverAddress,
		          sizeof( serverAddress ) ) == -1 )
		{
			write( 2, BIND_ERROR, sizeof( BIND_ERROR ) );
		}

		if( listen( serverSocket, 5 ) == -1 )
		{
			write( 2, LISTEN_ERROR, sizeof( LISTEN_ERROR ) );
		}

		while( 1 )
		{
			addressSize  = sizeof( clientAddress );
			clientSocket = accept(
			    serverSocket, ( struct sockaddr* ) &clientAddress,
			    &addressSize );
			if( clientSocket == -1 )
				continue;
			else { write( 1, NEW_CLIENT, sizeof( NEW_CLIENT ) ); }
			pid_t pid = fork();
			if( pid == -1 )
			{
				write( 2, FORK_FAILED, sizeof( FORK_FAILED ) );
				close( clientSocket );
				continue;
			}
			else if( pid == 0 )
			{
				close( serverSocket );
				int str_len;
				while( ( str_len = read( clientSocket, buffer,
				                         BUFFER_SIZE ) ) != 0 )
					write( clientSocket, buffer, str_len );
				close( clientSocket );
				write( 1, CLIENT_DISCONNECTED,
				       sizeof( CLIENT_DISCONNECTED ) );
				exit( 0 );
			}
			else { close( clientSocket ); }
		}
		close( serverSocket );
	}
	return 0;
}
