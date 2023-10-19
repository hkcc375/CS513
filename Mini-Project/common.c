#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "common.h"
#include "course_student_mapping.h"
#include "server_constants.h"
#include "socket_constants.h"

int read_student_record( int fileDescriptor, struct student* record, int record_indx, int record_size )
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
	else { write( 1, STUDENT_RECORD_READ_SUCCESSFUL, sizeof( STUDENT_RECORD_READ_SUCCESSFUL ) ); }
	return 1;
}

int read_faculty_record( int fileDescriptor, struct faculty* record, int record_indx, int record_size )
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
	else { write( 1, FACULTY_RECORD_READ_SUCCESSFUL, sizeof( FACULTY_RECORD_READ_SUCCESSFUL ) ); }
	return 1;
}

int read_course_record( int fileDescriptor, struct course* record, int record_indx, int record_size )
{
	lseek( fileDescriptor, ( record_indx - 1 ) * record_size, SEEK_SET );
	int read_bytes = read( fileDescriptor, record, record_size );
	if( read_bytes == -1 )
		perror( COURSE_RECORD_READ_FAILED );
	else if( read_bytes == 0 )
	{
		write( 1, COURSE_RECORD_EOF, sizeof( COURSE_RECORD_EOF ) );
		return 0;
	}
	else { write( 1, COURSE_RECORD_READ_SUCCESSFUL, sizeof( COURSE_RECORD_READ_SUCCESSFUL ) ); }
	return 1;
}

int read_mapping_record( int fileDescriptor, struct mapping* record, int record_indx, int record_size )
{
	lseek( fileDescriptor, ( record_indx - 1 ) * record_size, SEEK_SET );
	int read_bytes = read( fileDescriptor, record, record_size );
	if( read_bytes == -1 )
		perror( MAPPING_RECORD_READ_FAILED );
	else if( read_bytes == 0 )
	{
		write( 1, MAPPING_RECORD_EOF, sizeof( MAPPING_RECORD_EOF ) );
		return 0;
	}
	else { write( 1, MAPPING_RECORD_READ_SUCCESSFUL, sizeof( MAPPING_RECORD_READ_SUCCESSFUL ) ); }
	return 1;
}

void write_student_record( int fileDescriptor, struct student* record, int record_indx, int record_size, int flag )
{
	if( flag == 0 && record_indx == 0 ) { lseek( fileDescriptor, 0, SEEK_END ); }
	else { lseek( fileDescriptor, ( record_indx - 1 ) * record_size, SEEK_SET ); }
	int write_bytes = write( fileDescriptor, record, record_size );
	if( write_bytes == -1 )
		perror( STUDENT_RECORD_WRITE_FAILED );
	else if( write_bytes == 0 )
		write( 1, STUDENT_RECORD_NOTHING_WRITTEN, sizeof( STUDENT_RECORD_NOTHING_WRITTEN ) );
	else { write( 1, STUDENT_RECORD_WRITE_SUCCESSFUL, sizeof( STUDENT_RECORD_WRITE_SUCCESSFUL ) ); }
}

void write_faculty_record( int fileDescriptor, struct faculty* record, int record_indx, int record_size, int flag )
{
	if( flag == 0 && record_indx == 0 ) { lseek( fileDescriptor, 0, SEEK_END ); }
	else { lseek( fileDescriptor, ( record_indx - 1 ) * record_size, SEEK_SET ); }
	int write_bytes = write( fileDescriptor, record, record_size );
	if( write_bytes == -1 )
		perror( FACULTY_RECORD_WRITE_FAILED );
	else if( write_bytes == 0 )
		write( 1, FACULTY_RECORD_NOTHING_WRITTEN, sizeof( FACULTY_RECORD_NOTHING_WRITTEN ) );
	else { write( 1, FACULTY_RECORD_WRITE_SUCCESSFUL, sizeof( FACULTY_RECORD_WRITE_SUCCESSFUL ) ); }
}

void write_course_record( int fileDescriptor, struct course* record, int record_indx, int record_size, int flag )
{
	if( flag == 0 && record_indx == 0 ) { lseek( fileDescriptor, 0, SEEK_END ); }
	else { lseek( fileDescriptor, ( record_indx - 1 ) * record_size, SEEK_SET ); }
	int write_bytes = write( fileDescriptor, record, record_size );
	if( write_bytes == -1 )
		perror( COURSE_RECORD_WRITE_FAILED );
	else if( write_bytes == 0 )
		write( 1, COURSE_RECORD_NOTHING_WRITTEN, sizeof( COURSE_RECORD_NOTHING_WRITTEN ) );
	else { write( 1, COURSE_RECORD_WRITE_SUCCESSFUL, sizeof( COURSE_RECORD_WRITE_SUCCESSFUL ) ); }
}

void write_mapping_record( int fileDescriptor, struct mapping* record, int record_indx, int record_size, int flag )
{
	if( flag == 0 && record_indx == 0 ) { lseek( fileDescriptor, 0, SEEK_END ); }
	else { lseek( fileDescriptor, ( record_indx - 1 ) * record_size, SEEK_SET ); }
	int write_bytes = write( fileDescriptor, record, record_size );
	if( write_bytes == -1 )
		perror( MAPPING_RECORD_WRITE_FAILED );
	else if( write_bytes == 0 )
		write( 1, MAPPING_RECORD_NOTHING_WRITTEN, sizeof( MAPPING_RECORD_NOTHING_WRITTEN ) );
	else { write( 1, MAPPING_RECORD_WRITE_SUCCESSFUL, sizeof( MAPPING_RECORD_WRITE_SUCCESSFUL ) ); }
}

int isRowEmpty( const char array[][COURSEID_LENGTH], int row )
{
	char emptyRow[COURSEID_LENGTH];
	memset( emptyRow, '\0', COURSEID_LENGTH );                   // Create a row of null characters
	return memcmp( array[row], emptyRow, COURSEID_LENGTH ) == 0; // Compare the row with an empty row
}

// int isStructEmpty( const struct mapping* s )
// {
// 	struct mapping mapping_record;
// 	memset( &mapping_record, 0, sizeof( struct mapping ) );

// 	return memcmp( s, &mapping_record, sizeof( struct mapping ) ) == 0;
// }

void saveVariablesToFile()
{
	int fileDescriptor = open( "globals.txt", O_CREAT | O_WRONLY, 0777 );
	if( fileDescriptor == -1 )
	{
		perror( "Error opening file for writing" );
		return;
	}
	write( fileDescriptor, &no_of_students, sizeof( int ) );
	write( fileDescriptor, &no_of_faculty, sizeof( int ) );
	write( fileDescriptor, &no_of_courses, sizeof( int ) );
	write( fileDescriptor, &no_of_mappings, sizeof( int ) );
	close( fileDescriptor );
}

void loadVariablesFromFile()
{
	int fileDescriptor = open( "globals.txt", O_RDONLY );
	if( fileDescriptor == -1 )
	{
		perror( "Error opening file for reading" );
		return;
	}
	read( fileDescriptor, &no_of_students, sizeof( int ) );
	read( fileDescriptor, &no_of_faculty, sizeof( int ) );
	read( fileDescriptor, &no_of_courses, sizeof( int ) );
	read( fileDescriptor, &no_of_mappings, sizeof( int ) );
	close( fileDescriptor );
}
