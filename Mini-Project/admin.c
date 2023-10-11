#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "admin.h"
#include "faculty.h"
#include "server_constants.h"
#include "socket_constants.h"
#include "student.h"

int no_of_students = 0;
int no_of_faculty  = 0;

void add_faculty( int clientSocket ) {}
void view_faculty_details( int clientSocket ) {}
void view_student_details( int clientSocket ) {}
void block_student( int clientSocket ) {}
void activate_student( int clientSocket ) {}
void logout_and_exit( int clientSocket ) {}
void modify_student_details( int clientSocket ) {}
void modify_faculty_details( int clientSocket ) {}

void add_student( int clientSocket )
{
	char *read_buffer, *write_buffer;
	read_buffer = ( char* ) malloc( 512 * sizeof( char ) );
	memset( read_buffer, 0, 512 );
	int fileDescriptor = open( "new.txt", O_CREAT | O_EXCL | O_RDWR, 0777 );

	if( no_of_students < MAX_STUDENTS )
	{
		struct student new_student;

		// Name
		write( clientSocket, ENTER_STUDENT_NAME,
		       sizeof( ENTER_STUDENT_NAME ) );
		sleep( 1 );
		int read_name_bytes = read( clientSocket, read_buffer, 512 );
		if( read_name_bytes == -1 )
			perror( "Could not read student name." );
		read_buffer[read_name_bytes - 1] = '\0';
		memset( new_student.name, 0, STUDENT_NAME_LENGTH );
		strcpy( new_student.name, read_buffer );
		printf( "%s\n", new_student.name );
		memset( read_buffer, 0, 512 );

		// Age
		write( clientSocket, ENTER_STUDENT_AGE,
		       sizeof( ENTER_STUDENT_AGE ) );
		sleep( 1 );
		int read_age_bytes = read( clientSocket, read_buffer, 512 );
		if( read_age_bytes == -1 )
			perror( "Could not read student age." );
		read_buffer[read_age_bytes - 1] = '\0';
		int age                         = atoi( read_buffer );
		new_student.age                 = age;
		printf( "%d\n", new_student.age );
		memset( read_buffer, 0, 512 );

		// Email
		write( clientSocket, ENTER_STUDENT_EMAIL,
		       sizeof( ENTER_STUDENT_EMAIL ) );
		sleep( 1 );
		int read_email_bytes = read( clientSocket, read_buffer, 512 );
		if( read_email_bytes == -1 )
			perror( "Could not read student email." );
		read_buffer[read_email_bytes - 1] = '\0';
		memset( new_student.email, 0, EMAIL_LENGTH );
		strcpy( new_student.email, read_buffer );
		printf( "%s\n", new_student.email );
		memset( read_buffer, 0, 512 );

		// Address
		write( clientSocket, ENTER_STUDENT_ADDRESS,
		       sizeof( ENTER_STUDENT_ADDRESS ) );
		sleep( 1 );
		int read_address_bytes = read( clientSocket, read_buffer, 512 );
		if( read_address_bytes == -1 )
			perror( "Could not read student address." );
		read_buffer[read_address_bytes - 1] = '\0';
		memset( new_student.address, 0, ADDRESS_LENGTH );
		strcpy( new_student.address, read_buffer );
		printf( "%s\n", new_student.address );
		memset( read_buffer, 0, 512 );

		// Password
		strcpy( new_student.password, "student" );
		printf( "%s\n", new_student.password );

		// Status
		new_student.status = 1;

		// Student ID
		// Apply Semaphores here..
		no_of_students++;
		new_student.student_id = no_of_students;

		// Username
		memset( new_student.username, 0, USERNAME_LENGTH );
		snprintf( new_student.username, USERNAME_LENGTH, "MT%03d",
		          new_student.student_id );
		printf( "%s\n", new_student.username );

		write( fileDescriptor, &new_student, sizeof( student ) );
		close( fileDescriptor );
	}
	else
	{
		write( clientSocket, CANNOT_ADD_STUDENTS,
		       sizeof( CANNOT_ADD_STUDENTS ) );
	}
}

void admin_menu_handler( int clientSocket )
{
	char *read_buffer, *write_buffer;
	read_buffer  = ( char* ) malloc( 512 * sizeof( char ) );
	write_buffer = ( char* ) malloc( 512 * sizeof( char ) );
	memset( read_buffer, 0, 512 );
	memset( write_buffer, 0, 512 );

	write( 1, WRITE_ADMIN_PROMPT_SUCCESSFUL,
	       sizeof( WRITE_ADMIN_PROMPT_SUCCESSFUL ) );
	ssize_t read_bytes =
	    read( clientSocket, read_buffer, 512 ); // Read Admin Choice

	if( read_bytes == -1 )
	{
		write( 2, READ_ADMIN_CHOICE_UNSUCCESSFUL,
		       sizeof( READ_ADMIN_CHOICE_UNSUCCESSFUL ) );
	}
	else if( read_bytes == 0 )
	{
		write( 1, READ_ADMIN_NO_CHOICE,
		       sizeof( READ_ADMIN_NO_CHOICE ) );
	}
	else
	{
		write( 1, read_buffer, strlen( read_buffer ) );
		int userChoice = atoi( read_buffer );
		switch( userChoice )
		{
		case 1: add_student( clientSocket ); break;
		case 2: add_faculty( clientSocket ); break;
		case 3: view_student_details( clientSocket ); break;
		case 4: view_faculty_details( clientSocket ); break;
		case 5: activate_student( clientSocket ); break;
		case 6: block_student( clientSocket ); break;
		case 7: modify_student_details( clientSocket ); break;
		case 8: modify_faculty_details( clientSocket ); break;
		case 9: logout_and_exit( clientSocket ); break;
		default: break;
		}
	}
	free( read_buffer );
	free( write_buffer );
}

void admin_connection_handler( int clientSocket )
{
	char *username_buffer, *password_buffer;
	username_buffer = ( char* ) malloc( 16 * sizeof( char ) );
	password_buffer = ( char* ) malloc( 16 * sizeof( char ) );
	while( 1 )
	{
		memset( username_buffer, 0, 16 );
		memset( password_buffer, 0, 16 );
		sleep( 1 );
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
			free( username_buffer );
			free( password_buffer );
			write( clientSocket, AUTHENTICATION_SUCCESS,
			       sizeof( AUTHENTICATION_SUCCESS ) );
			sleep( 1 );
			write( clientSocket, ADMIN_MENU, sizeof( ADMIN_MENU ) );
			break;
		}
		else
		{
			write( clientSocket, AUTHENTICATION_FAILED,
			       sizeof( AUTHENTICATION_FAILED ) );
		}
	}
	admin_menu_handler( clientSocket );
}