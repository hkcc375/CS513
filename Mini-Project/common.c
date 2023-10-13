#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "common.h"
#include "socket_constants.h"

int read_student_record( int fileDescriptor, struct student* record,
                         int record_indx, int record_size )
{
	lseek( fileDescriptor, ( record_indx - 1 ) * record_size, SEEK_SET );
	int read_bytes = read( fileDescriptor, record, record_size );
	if( read_bytes == -1 )
		perror( STUDENT_RECORD_READ_FAILED );
	else if( read_bytes == 0 )
	{
		write( 1, STUDENT_RECORD_EOF, sizeof( STUDENT_RECORD_EOF ) );
		return 0;
	}
	else
	{
		write( 1, STUDENT_RECORD_READ_SUCCESSFUL,
		       sizeof( STUDENT_RECORD_READ_SUCCESSFUL ) );
	}
	return 1;
}

int read_faculty_record( int fileDescriptor, struct faculty* record,
                         int record_indx, int record_size )
{
	lseek( fileDescriptor, ( record_indx - 1 ) * record_size, SEEK_SET );
	int read_bytes = read( fileDescriptor, record, record_size );
	if( read_bytes == -1 )
		perror( FACULTY_RECORD_READ_FAILED );
	else if( read_bytes == 0 )
	{
		write( 1, FACULTY_RECORD_EOF, sizeof( FACULTY_RECORD_EOF ) );
		return 0;
	}
	else
	{
		write( 1, FACULTY_RECORD_READ_SUCCESSFUL,
		       sizeof( FACULTY_RECORD_READ_SUCCESSFUL ) );
	}
	return 1;
}

void write_student_record( int fileDescriptor, struct student* record,
                           int record_indx, int record_size, int flag )
{
	if( flag == 0 && record_indx == 0 )
	{
		lseek( fileDescriptor, 0, SEEK_END );
	}
	else
	{
		lseek( fileDescriptor, ( record_indx - 1 ) * record_size,
		       SEEK_SET );
	}
	int write_bytes = write( fileDescriptor, record, record_size );
	if( write_bytes == -1 )
		perror( STUDENT_RECORD_WRITE_FAILED );
	else if( write_bytes == 0 )
		write( 1, STUDENT_RECORD_NOTHING_WRITTEN,
		       sizeof( STUDENT_RECORD_NOTHING_WRITTEN ) );
	else
	{
		write( 1, STUDENT_RECORD_WRITE_SUCCESSFUL,
		       sizeof( STUDENT_RECORD_WRITE_SUCCESSFUL ) );
	}
}

void write_faculty_record( int fileDescriptor, struct faculty* record,
                           int record_indx, int record_size, int flag )
{
	if( flag == 0 && record_indx == 0 )
	{
		lseek( fileDescriptor, 0, SEEK_END );
	}
	else
	{
		lseek( fileDescriptor, ( record_indx - 1 ) * record_size,
		       SEEK_SET );
	}
	int write_bytes = write( fileDescriptor, record, record_size );
	if( write_bytes == -1 )
		perror( FACULTY_RECORD_WRITE_FAILED );
	else if( write_bytes == 0 )
		write( 1, FACULTY_RECORD_NOTHING_WRITTEN,
		       sizeof( FACULTY_RECORD_NOTHING_WRITTEN ) );
	else
	{
		write( 1, FACULTY_RECORD_WRITE_SUCCESSFUL,
		       sizeof( FACULTY_RECORD_WRITE_SUCCESSFUL ) );
	}
}

void write_course_record( int fileDescriptor, struct course* record,
                          int record_indx, int record_size, int flag )
{
}