#include <arpa/inet.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <unistd.h>

#include "server_constants.h"
#include "socket_constants.h"

void reap_child_process( int signal );
void client_connection_handler( int clientSocket );

void reap_child_process( int signal )
{
	int status;
	pid_t childPid = waitpid( -1, &status, WNOHANG );
	if( childPid == -1 )
	{
		write( 2, PROCESS_REAPED_UNSUCCESSFUL,
		       sizeof( PROCESS_REAPED_UNSUCCESSFUL ) );
	}
	else if( childPid == 0 )
	{
		write( 1, PROCESS_STATUS_NOTAVAILABLE,
		       sizeof( PROCESS_STATUS_NOTAVAILABLE ) );
	}
	else
	{
		write( 1, PROCESS_REAPED_SUCCESSFUL,
		       sizeof( PROCESS_REAPED_SUCCESSFUL ) );
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
			else if( pid == 0 ) // Child Process
			{
				close( serverSocket );
				int str_len;

				// Client Connection Handler
				char *read_buffer, *write_buffer;
				read_buffer =
				    ( char* ) malloc( 512 * sizeof( char ) );
				write_buffer =
				    ( char* ) malloc( 512 * sizeof( char ) );

				ssize_t write_bytes =
				    write( clientSocket, INITIAL_PROMPT,
				           sizeof( INITIAL_PROMPT ) );
				if( write_bytes == -1 )
					write( 2, WRITE_UNSUCCESSFUL,
					       sizeof( WRITE_UNSUCCESSFUL ) );
				else
				{
					write( 1, WRITE_SUCCESSFUL,
					       sizeof( WRITE_SUCCESSFUL ) );

					memset( &read_buffer, 0,
					        sizeof( read_buffer ) );

					ssize_t read_bytes =
					    read( clientSocket, read_buffer,
					          sizeof( read_buffer ) );

					if( read_bytes == -1 )
					{
						write(
						    2, READ_UNSUCCESSFUL,
						    sizeof(
						        READ_UNSUCCESSFUL ) );
					}
					else if( read_bytes == 0 )
					{
						write(
						    1, READ_NOCHOICE,
						    sizeof( READ_NOCHOICE ) );
					}
					else
					{
						int userChoice =
						    atoi( read_buffer );
						printf(
						    "User Choice is : %d \n",
						    userChoice );
					}
				}
				close( clientSocket );
				free( read_buffer );
				free( write_buffer );
				write( 1, CLIENT_DISCONNECTED,
				       sizeof( CLIENT_DISCONNECTED ) );
				exit( 0 );
			}
			else // Parent Processs
			{
				close( clientSocket );
			}
		}
		close( serverSocket );
	}
	return 0;
}
