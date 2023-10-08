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
void reapChildProcess( int signal );

void error_handling( char* message )
{
	fputs( message, stderr );
	fputc( '\n', stderr );
	exit( 1 );
}

void reapChildProcess( int signal )
{
	int status;
	pid_t childPid = waitpid( -1, &status, WNOHANG );
	if( childPid == -1 )
		perror( "Waitpid was unsuccessful. \n" );
	else if( childPid == 0 )
		printf( "There was at least one child whose status information "
		        "could not be obtained. \n" );
	else
	{
		printf( "Child Process was successfully reaped. \n" );
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
		printf( "Usage : %s <port-no>", argv[0] );
	else
	{
		signal( SIGCHLD, reapChildProcess );

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

		while( 1 )
		{
			addressSize  = sizeof( clientAddress );
			clientSocket = accept(
			    serverSocket, ( struct sockaddr* ) &clientAddress,
			    &addressSize );
			if( clientSocket == -1 )
				continue;
			else
				puts( "New client has connected to the "
				      "server." );
			pid_t pid = fork();
			if( pid == -1 )
			{
				perror( "fork() failed. \n" );
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
				puts( "Client disconnected." );
				exit( 0 );
			}
			else { close( clientSocket ); }
		}
		close( serverSocket );
	}
	return 0;
}
