#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <unistd.h>

int main()
{
	key_t queueKey;
	int queueIdentifier;
	int msgctlStatus;

	queueKey = ftok( ".", 1 );

	if( queueKey == -1 )
	{
		perror( "Error while computing key!" );
		_exit( 0 );
	}

	queueIdentifier = msgget( queueKey, IPC_CREAT | 0700 );
	if( queueIdentifier == -1 )
	{
		perror( "Error while creating Message Queue!" );
		_exit( 0 );
	}

	printf( "Key: %d\n", queueKey );
	printf( "Message Queue Identifier: %d\n\n", queueIdentifier );

	msgctlStatus = msgctl( queueIdentifier, IPC_RMID, NULL );

	if( msgctlStatus == -1 )
	{
		perror( "Error while removing Message Queue" );
		_exit( 0 );
	}
}
