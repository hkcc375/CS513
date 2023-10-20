#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <unistd.h>

int main()
{
	key_t shmKey;
	int shmIdentifier;
	ssize_t shmSize = 20;
	char *shmPointer, *rdOnlyShmPointer;

	shmKey = ftok( ".", 1 );

	if( shmKey == -1 )
	{
		perror( "Error while computing key!" );
		_exit( 0 );
	}

	shmIdentifier = shmget( shmKey, shmSize, IPC_CREAT | 0700 );

	if( shmIdentifier == -1 )
	{
		perror( "Error while getting Shared Memory!" );
		_exit( 0 );
	}
	// ======================== 1 ========================
	shmPointer = shmat( shmIdentifier, ( void * ) 0, 0 );

	if( *shmPointer == -1 )
	{
		perror( "Error while attaching address space!" );
		_exit( 0 );
	}

	printf( "Press enter to write to the shared memory!\n" );
	getchar();

	sprintf( shmPointer, "Yolo" ); // Writing to the shared memory

	printf( "Press enter to read from the shared memory!\n" );
	getchar();

	printf( "Data from shared memory: %s\n", shmPointer );

	// ===================================================

	// ======================== 2 ========================

	rdOnlyShmPointer = shmat( shmIdentifier, ( void * ) 0, SHM_RDONLY );
	if( *rdOnlyShmPointer == -1 )
	{
		perror( "Error while attaching address space!" );
		_exit( 0 );
	}

	printf( "Press enter to read from the shared memory!\n" );
	getchar();

	printf( "Data from shared memory: %s\n", rdOnlyShmPointer );

	// ===================================================

	// ======================== 3 ========================
	printf( "Detaching pointer to shared memory!\n" );
	shmdt( shmPointer );       // Dettach pointer to Shared Memory
	shmdt( rdOnlyShmPointer ); // Dettach pointer to Shared Memory
	// ===================================================

	// ======================== 4 ========================
	printf( "Press enter to delete the shared memory!\n" );
	getchar();

	// Delete Shared Memory
	shmctl( shmIdentifier, IPC_RMID, NULL );
	// ===================================================
}