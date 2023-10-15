#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "common.h"
#include "server_constants.h"
#include "socket_constants.h"
#include "student.h"

void enroll_course( int clientSocket, int student_id ) {}
void drop_course( int clientSocket, int student_id ) {}
void view_all_courses( int clientSocket, int student_id ) {}
void view_enrolled_courses( int clientSocket, int student_id ) {}

void change_password_student( int clientSocket, int student_id )
{
	struct student record;
	char *password_buffer, *confirm_password_buffer;
	password_buffer = ( char* ) malloc( PASSWORD_LENGTH * sizeof( char ) );
	confirm_password_buffer =
	    ( char* ) malloc( PASSWORD_LENGTH * sizeof( char ) );
	memset( password_buffer, 0, PASSWORD_LENGTH );
	memset( confirm_password_buffer, 0, PASSWORD_LENGTH );

	int fileDescriptor = open( "student.txt", O_CREAT | O_RDWR, 0777 );
	if( fileDescriptor == -1 )
		perror( STUDENT_FILE_OPEN_FAILED );
	else
	{
		while( 1 )
		{
			write( clientSocket, ENTER_PASSWORD,
			       sizeof( ENTER_PASSWORD ) );
			sleep( 1 );
			int read_student_password_bytes = read(
			    clientSocket, password_buffer, PASSWORD_LENGTH );
			if( read_student_password_bytes == -1 )
				perror( "Reading from socket failed. \n" );
			password_buffer[read_student_password_bytes - 1] = '\0';
			write( 1, password_buffer, strlen( password_buffer ) );

			write( clientSocket, ENTER_PASSWORD_CONFIRM,
			       sizeof( ENTER_PASSWORD_CONFIRM ) );
			sleep( 1 );
			int read_student_password_confirm_bytes =
			    read( clientSocket, confirm_password_buffer,
			          PASSWORD_LENGTH );
			if( read_student_password_confirm_bytes == -1 )
				perror( "Reading from socket failed. \n" );
			confirm_password_buffer
			    [read_student_password_confirm_bytes - 1] = '\0';
			write( 1, confirm_password_buffer,
			       strlen( confirm_password_buffer ) );

			int retval = read_student_record(
			    fileDescriptor, &record, student_id,
			    sizeof( struct student ) );

			if( !strcmp( password_buffer,
			             confirm_password_buffer ) &&
			    retval == 1 )
			{
				write( clientSocket, PASSWORDS_MATCH,
				       sizeof( PASSWORDS_MATCH ) );
				write( 1, PASSWORDS_MATCH,
				       sizeof( PASSWORDS_MATCH ) );
				sleep( 1 );
				write( clientSocket, PASSWORDS_CHANGED_CLIENT,
				       sizeof( PASSWORDS_CHANGED_CLIENT ) );
				write( 1, STUDENT_PASSWORDS_CHANGED_SERVER,
				       sizeof(
				           STUDENT_PASSWORDS_CHANGED_SERVER ) );

				// Changing Password
				memset( record.password, 0, PASSWORD_LENGTH );
				strcpy( record.password, password_buffer );

				write_student_record( fileDescriptor, &record,
				                      student_id,
				                      sizeof( student ), 1 );
				break;
			}
			else
			{
				write( clientSocket, PASSWORDS_DO_NOT_MATCH,
				       sizeof( PASSWORDS_DO_NOT_MATCH ) );
				write( 1, PASSWORDS_DO_NOT_MATCH,
				       sizeof( PASSWORDS_DO_NOT_MATCH ) );
				sleep( 1 );
			}
		}
	}
	close( fileDescriptor );
	free( password_buffer );
	free( confirm_password_buffer );
}

void student_menu_handler( int clientSocket, int student_id )
{
	int condition = 1;
	while( condition )
	{
		char* read_buffer;
		read_buffer = ( char* ) malloc( 512 * sizeof( char ) );
		memset( read_buffer, 0, 512 );
		write( 1, WRITE_STUDENT_PROMPT_SUCCESSFUL,
		       sizeof( WRITE_STUDENT_PROMPT_SUCCESSFUL ) );
		ssize_t read_bytes = read( clientSocket, read_buffer, 512 );

		if( read_bytes == -1 )
		{
			write( 2, READ_STUDENT_CHOICE_UNSUCCESSFUL,
			       sizeof( READ_STUDENT_CHOICE_UNSUCCESSFUL ) );
		}
		else
		{
			write( 1, read_buffer, strlen( read_buffer ) );
			int userChoice = atoi( read_buffer );
			if( userChoice == 0 )
			{
				write( 1, STUDENT_NO_CHOICE_SERVER,
				       sizeof( STUDENT_NO_CHOICE_SERVER ) );
				write( clientSocket, STUDENT_NO_CHOICE_CLIENT,
				       sizeof( STUDENT_NO_CHOICE_CLIENT ) );
			}
			else
			{
				switch( userChoice )
				{
				case 1:
					add_new_course( clientSocket,
					                student_id );
					break;
				case 2:
					view_offering_courses( clientSocket,
					                       student_id );
					break;
				case 3:
					remove_course_from_catalog(
					    clientSocket, student_id );
					break;
				case 4:
					update_course( clientSocket,
					               student_id );
					break;
				case 5:
					change_password_faculty( clientSocket,
					                         student_id );
					break;
				case 6:
					logout_and_exit_faculty();
					condition = 0;
					break;
				default:
					write(
					    clientSocket,
					    STUDENT_INCORRECT_CHOICE_CLIENT,
					    sizeof(
					        STUDENT_INCORRECT_CHOICE_CLIENT ) );
					write(
					    1, STUDENT_INCORRECT_CHOICE_SERVER,
					    sizeof(
					        STUDENT_INCORRECT_CHOICE_SERVER ) );
					break;
				}
			}
		}
		sleep( 1 );
		if( condition == 1 )
			write( clientSocket, STUDENT_MENU,
			       sizeof( STUDENT_MENU ) );
		free( read_buffer );
	}
}

void logout_and_exit_student() { return; }

void student_connection_handler( int clientSocket )
{
	char *username_buffer, *password_buffer;
	struct student record;
	username_buffer = ( char* ) malloc( USERNAME_LENGTH * sizeof( char ) );
	password_buffer = ( char* ) malloc( PASSWORD_LENGTH * sizeof( char ) );
	int fileDescriptor = open( "student.txt", O_RDONLY, 0777 );
	if( fileDescriptor == -1 ) { perror( STUDENT_FILE_OPEN_FAILED ); }
	else
	{
		int student_id;
		while( 1 )
		{
			memset( username_buffer, 0, USERNAME_LENGTH );
			memset( password_buffer, 0, PASSWORD_LENGTH );
			sleep( 1 );
			write( clientSocket, LOGIN_MESSAGE,
			       sizeof( LOGIN_MESSAGE ) );
			int read_username_bytes = read(
			    clientSocket, username_buffer, USERNAME_LENGTH );
			username_buffer[read_username_bytes - 1] = '\0';

			char* temp_username_buffer = ( char* ) malloc(
			    USERNAME_LENGTH * sizeof( char ) );
			memset( temp_username_buffer, 0, USERNAME_LENGTH );
			strcpy( temp_username_buffer,
			        username_buffer + strlen( username_buffer ) -
			            3 );
			student_id = atoi( temp_username_buffer );
			free( temp_username_buffer );
			if( student_id == 0 )
			{
				write(
				    clientSocket, AUTHENTICATION_USERNAME_EMPTY,
				    sizeof( AUTHENTICATION_USERNAME_EMPTY ) );
			}
			else
			{
				int retval = read_student_record(
				    fileDescriptor, &record, student_id,
				    sizeof( struct faculty ) );
				if( retval == 1 )
				{

					// Comment out the below line...
					write( 1, username_buffer,
					       strlen( username_buffer ) );

					write( clientSocket, PASSWORD_MESSAGE,
					       sizeof( PASSWORD_MESSAGE ) );
					int read_password_bytes =
					    read( clientSocket, password_buffer,
					          PASSWORD_LENGTH );

					write( 1, password_buffer,
					       strlen( password_buffer ) );
					password_buffer[read_password_bytes -
					                1] = '\0';

					if( !( strcmp( record.password,
					               password_buffer ) ) )
					{
						free( username_buffer );
						free( password_buffer );
						close( fileDescriptor );
						write(
						    clientSocket,
						    AUTHENTICATION_SUCCESS,
						    sizeof(
						        AUTHENTICATION_SUCCESS ) );
						sleep( 1 );
						write( clientSocket,
						       STUDENT_MENU,
						       sizeof( STUDENT_MENU ) );
						break;
					}
					else
					{
						write(
						    clientSocket,
						    AUTHENTICATION_FAILED,
						    sizeof(
						        AUTHENTICATION_FAILED ) );
					}
				}
				else
				{
					write( clientSocket,
					       STUDENT_RECORD_NOT_FOUND,
					       sizeof(
					           STUDENT_RECORD_NOT_FOUND ) );
				}
			}
		}
		student_menu_handler( clientSocket, student_id );
	}
}