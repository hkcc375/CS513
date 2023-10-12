#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "admin.h"
#include "common.h"
#include "faculty.h"
#include "server_constants.h"
#include "socket_constants.h"
#include "student.h"

// Maybe I will have to maintain this in another file.
int no_of_students = 0;
int no_of_faculty  = 0;

void add_faculty( int clientSocket ) {}
void view_faculty_details( int clientSocket ) {}
void modify_faculty_details( int clientSocket ) {}

void logout_and_exit( int clientSocket ) {}

void modify_student_details( int clientSocket ) {}

void activate_student( int clientSocket )
{
	struct student record;
	char* read_buffer;
	read_buffer = ( char* ) malloc( 512 * sizeof( char ) );
	memset( read_buffer, 0, 512 );

	int fileDescriptor = open( "student.txt", O_CREAT | O_RDWR, 0777 );
	if( fileDescriptor == -1 )
		perror( STUDENT_FILE_OPEN_FAILED );
	else
	{
		write( clientSocket, ENTER_STUDENT_ID,
		       sizeof( ENTER_STUDENT_ID ) );
		sleep( 1 );
		int read_student_id_bytes =
		    read( clientSocket, read_buffer, 512 );
		if( read_student_id_bytes == -1 )
			perror( "Reading from socket failed. \n" );
		read_buffer[read_student_id_bytes - 1] = '\0';
		int student_id                         = atoi( read_buffer );
		if( student_id == 0 )
		{
			write( clientSocket, STUDENT_RECORD_INVALID,
			       sizeof( STUDENT_RECORD_INVALID ) );
			close( fileDescriptor );
		}
		else
		{
			int retval = read_student_record(
			    fileDescriptor, &record, student_id,
			    sizeof( struct student ) );
			if( retval == 1 )
			{
				memset( read_buffer, 0, 512 );

				// Probably, we might have to apply semaphore
				// here.
				record.status = 1;

				write_student_record( fileDescriptor, &record,
				                      student_id,
				                      sizeof( student ), 1 );
				write( clientSocket, STUDENT_RECORD_ACTIVATED,
				       sizeof( STUDENT_RECORD_ACTIVATED ) );
			}
			else
			{
				write( clientSocket, STUDENT_RECORD_NOT_FOUND,
				       sizeof( STUDENT_RECORD_NOT_FOUND ) );
			}
		}
	}
	close( fileDescriptor );
	free( read_buffer );
}

void block_student( int clientSocket )
{
	struct student record;
	char* read_buffer;
	read_buffer = ( char* ) malloc( 512 * sizeof( char ) );
	memset( read_buffer, 0, 512 );

	int fileDescriptor = open( "student.txt", O_CREAT | O_RDWR, 0777 );
	if( fileDescriptor == -1 )
		perror( STUDENT_FILE_OPEN_FAILED );
	else
	{
		write( clientSocket, ENTER_STUDENT_ID,
		       sizeof( ENTER_STUDENT_ID ) );
		sleep( 1 );
		int read_student_id_bytes =
		    read( clientSocket, read_buffer, 512 );
		if( read_student_id_bytes == -1 )
			perror( "Reading from socket failed. \n" );
		read_buffer[read_student_id_bytes - 1] = '\0';
		int student_id                         = atoi( read_buffer );
		if( student_id == 0 )
		{
			write( clientSocket, STUDENT_RECORD_INVALID,
			       sizeof( STUDENT_RECORD_INVALID ) );
			close( fileDescriptor );
		}
		else
		{
			int retval = read_student_record(
			    fileDescriptor, &record, student_id,
			    sizeof( struct student ) );
			if( retval == 1 )
			{
				memset( read_buffer, 0, 512 );

				// Probably, we might have to apply semaphore
				// here.
				record.status = 0;

				write_student_record( fileDescriptor, &record,
				                      student_id,
				                      sizeof( student ), 1 );
				write( clientSocket, STUDENT_RECORD_BLOCKED,
				       sizeof( STUDENT_RECORD_BLOCKED ) );
			}
			else
			{
				write( clientSocket, STUDENT_RECORD_NOT_FOUND,
				       sizeof( STUDENT_RECORD_NOT_FOUND ) );
			}
		}
	}
	close( fileDescriptor );
	free( read_buffer );
}

void view_student_details( int clientSocket )
{
	struct student record;
	char* read_buffer;
	read_buffer = ( char* ) malloc( 512 * sizeof( char ) );
	memset( read_buffer, 0, 512 );

	int fileDescriptor = open( "student.txt", O_CREAT | O_RDONLY, 0777 );
	if( fileDescriptor == -1 )
		perror( STUDENT_FILE_OPEN_FAILED );
	else
	{
		write( clientSocket, ENTER_STUDENT_ID,
		       sizeof( ENTER_STUDENT_ID ) );
		sleep( 1 );
		int read_student_id_bytes =
		    read( clientSocket, read_buffer, 512 );
		if( read_student_id_bytes == -1 )
			perror( "Reading from socket failed. \n" );
		read_buffer[read_student_id_bytes - 1] = '\0';
		int student_id                         = atoi( read_buffer );
		if( student_id == 0 )
		{
			write( clientSocket, STUDENT_RECORD_INVALID,
			       sizeof( STUDENT_RECORD_INVALID ) );
		}
		else
		{
			int retval = read_student_record(
			    fileDescriptor, &record, student_id,
			    sizeof( struct student ) );
			if( retval == 1 )
			{
				memset( read_buffer, 0, 512 );
				// Name
				strcat( read_buffer, "Name : " );
				strcat( read_buffer, record.name );
				strcat( read_buffer, "\n" );
				write( clientSocket, read_buffer,
				       strlen( read_buffer ) );
				memset( read_buffer, 0, 512 );

				// Age
				strcat( read_buffer, "Age : " );
				char* age =
				    ( char* ) malloc( 3 * sizeof( char ) );
				snprintf( age, 3, "%d", record.age );
				strcat( read_buffer, age );
				strcat( read_buffer, "\n" );
				write( clientSocket, read_buffer,
				       strlen( read_buffer ) );
				memset( read_buffer, 0, 512 );
				free( age );

				// Email
				strcat( read_buffer, "Email : " );
				strcat( read_buffer, record.email );
				strcat( read_buffer, "\n" );
				write( clientSocket, read_buffer,
				       strlen( read_buffer ) );
				memset( read_buffer, 0, 512 );

				// Address
				strcat( read_buffer, "Address : " );
				strcat( read_buffer, record.address );
				strcat( read_buffer, "\n" );
				write( clientSocket, read_buffer,
				       strlen( read_buffer ) );
				memset( read_buffer, 0, 512 );

				// Username
				strcat( read_buffer, "Login id / Username : " );
				strcat( read_buffer, record.username );
				strcat( read_buffer, "\n" );
				write( clientSocket, read_buffer,
				       strlen( read_buffer ) );
				memset( read_buffer, 0, 512 );
			}
			else
			{
				write( clientSocket, STUDENT_RECORD_NOT_FOUND,
				       sizeof( STUDENT_RECORD_NOT_FOUND ) );
			}
		}
	}
	close( fileDescriptor );
	free( read_buffer );
}

void add_student( int clientSocket )
{
	char* read_buffer;
	read_buffer = ( char* ) malloc( 512 * sizeof( char ) );
	memset( read_buffer, 0, 512 );

	// Add checks to see if file already exists.
	int fileDescriptor = open( "student.txt", O_CREAT | O_WRONLY, 0777 );
	if( fileDescriptor == -1 )
		perror( STUDENT_FILE_OPEN_FAILED );
	else
	{
		if( no_of_students < MAX_STUDENTS )
		{
			struct student new_student;

			// Name
			write( clientSocket, ENTER_STUDENT_NAME,
			       sizeof( ENTER_STUDENT_NAME ) );
			sleep( 1 );
			int read_name_bytes =
			    read( clientSocket, read_buffer, 512 );
			if( read_name_bytes == -1 )
				perror( "Could not read student name." );
			read_buffer[read_name_bytes - 1] = '\0';
			memset( new_student.name, 0, STUDENT_NAME_LENGTH );
			strcpy( new_student.name, read_buffer );
			// printf( "%s\n", new_student.name );
			memset( read_buffer, 0, 512 );

			// Age
			write( clientSocket, ENTER_STUDENT_AGE,
			       sizeof( ENTER_STUDENT_AGE ) );
			sleep( 1 );
			int read_age_bytes =
			    read( clientSocket, read_buffer, 512 );
			if( read_age_bytes == -1 )
				perror( "Could not read student age." );
			read_buffer[read_age_bytes - 1] = '\0';
			int age                         = atoi( read_buffer );
			new_student.age                 = age;
			memset( read_buffer, 0, 512 );

			// Email
			write( clientSocket, ENTER_STUDENT_EMAIL,
			       sizeof( ENTER_STUDENT_EMAIL ) );
			sleep( 1 );
			int read_email_bytes =
			    read( clientSocket, read_buffer, 512 );
			if( read_email_bytes == -1 )
				perror( "Could not read student email." );
			read_buffer[read_email_bytes - 1] = '\0';
			memset( new_student.email, 0, EMAIL_LENGTH );
			strcpy( new_student.email, read_buffer );
			memset( read_buffer, 0, 512 );

			// Address
			write( clientSocket, ENTER_STUDENT_ADDRESS,
			       sizeof( ENTER_STUDENT_ADDRESS ) );
			sleep( 1 );
			int read_address_bytes =
			    read( clientSocket, read_buffer, 512 );
			if( read_address_bytes == -1 )
				perror( "Could not read student addsress." );
			read_buffer[read_address_bytes - 1] = '\0';
			memset( new_student.address, 0, ADDRESS_LENGTH );
			strcpy( new_student.address, read_buffer );
			memset( read_buffer, 0, 512 );

			// Password
			strcpy( new_student.password, "student" );

			// Status
			new_student.status = 1;

			// Student ID
			// Apply Semaphores here... OR maintain the details in
			// another file...
			no_of_students++;
			new_student.student_id = no_of_students;

			// Username
			memset( new_student.username, 0, USERNAME_LENGTH );
			snprintf( new_student.username, USERNAME_LENGTH,
			          "MT%03d", new_student.student_id );

			write( clientSocket, STUDENT_CREATED_SUCCESSFULLY,
			       sizeof( STUDENT_CREATED_SUCCESSFULLY ) );

			write_student_record( fileDescriptor, &new_student, 0,
			                      sizeof( student ), 0 );
		}
		else
		{
			write( clientSocket, CANNOT_ADD_STUDENTS,
			       sizeof( CANNOT_ADD_STUDENTS ) );
		}
	}
	free( read_buffer );
	close( fileDescriptor );
}

void admin_menu_handler( int clientSocket )
{
	char *read_buffer, *write_buffer;
	read_buffer  = ( char* ) malloc( 512 * sizeof( char ) );
	write_buffer = ( char* ) malloc( 512 * sizeof( char ) );

	while( 1 )
	{
		memset( read_buffer, 0, 512 );
		memset( write_buffer, 0, 512 );
		write( 1, WRITE_ADMIN_PROMPT_SUCCESSFUL,
		       sizeof( WRITE_ADMIN_PROMPT_SUCCESSFUL ) );
		// sleep( 2 );
		ssize_t read_bytes = read( clientSocket, read_buffer,
		                           512 ); // Read Admin Choice

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
			case 2: view_student_details( clientSocket ); break;
			case 3: add_faculty( clientSocket ); break;
			case 4: view_faculty_details( clientSocket ); break;
			case 5: activate_student( clientSocket ); break;
			case 6: block_student( clientSocket ); break;
			case 7: modify_student_details( clientSocket ); break;
			case 8: modify_faculty_details( clientSocket ); break;
			case 9: logout_and_exit( clientSocket ); break;
			default: break;
			}
		}
		write( clientSocket, ADMIN_MENU, sizeof( ADMIN_MENU ) );
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