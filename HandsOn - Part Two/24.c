#include <errno.h>
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <unistd.h>

int main()
{
	key_t queueKey;      // IPC (Message Queue) key
	int queueIdentifier; // IPC (Message Queue) identifier

	queueKey = ftok( ".", 1 );

	if( queueKey == -1 )
	{
		perror( "Error while computing key!" );
		_exit( 0 );
	}

	queueIdentifier = msgget( queueKey, IPC_CREAT | IPC_EXCL | 0700 );
	if( queueIdentifier == -1 )
	{
		perror( "Error while creating Message Queue!" );
		_exit( 0 );
	}

	printf( "Key: %d\n", queueKey );
	printf( "Message Queue Identifier: %d\n", queueIdentifier );
}