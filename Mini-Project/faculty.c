#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "common.h"
#include "faculty.h"
#include "server_constants.h"
#include "socket_constants.h"

// Maybe I will have to store this in another file.
int no_of_courses  = 0;
int no_of_mappings = 0;

void remove_course_from_catalog( int clientSocket, int faculty_id ) {}
void update_course( int clientSocket, int faculty_id ) {}

void view_offering_courses( int clientSocket, int faculty_id ) {}

void add_new_course( int clientSocket, int faculty_id )
{
	char* read_buffer;
	read_buffer = ( char* ) malloc( 512 * sizeof( char ) );
	memset( read_buffer, 0, 512 );

	int fileDescriptor_course = open( "course.txt", O_CREAT | O_WRONLY, 0777 );
	if( fileDescriptor_course == -1 )
		perror( COURSE_FILE_OPEN_FAILED );
	else
	{
		int fileDescriptor_faculty = open( "faculty.txt", O_CREAT | O_RDWR, 0777 );
		if( fileDescriptor_faculty == -1 )
			perror( FACULTY_FILE_OPEN_FAILED );
		else
		{
			struct faculty f_record;
			int retval = read_faculty_record( fileDescriptor_faculty, &f_record, faculty_id,
			                                  sizeof( struct faculty ) );
			if( retval == 1 )
			{
				if( f_record.no_of_offering_courses < MAX_COURSES_PER_FACULTY )
				{
					if( no_of_courses < MAX_COURSES )
					{
						struct course new_course;
						memset( read_buffer, 0, 512 );

						// Course Name
						write( clientSocket, ENTER_COURSE_NAME, sizeof( ENTER_COURSE_NAME ) );
						sleep( 1 );
						int read_course_name_bytes = read( clientSocket, read_buffer, 512 );
						if( read_course_name_bytes == -1 )
							perror( "Could not "
							        "read course "
							        "name." );
						read_buffer[read_course_name_bytes - 1] = '\0';
						memset( new_course.course_name, 0, NAME_LENGTH );
						strcpy( new_course.course_name, read_buffer );
						memset( read_buffer, 0, 512 );

						// Course Department
						write( clientSocket, ENTER_COURSE_DEPARTMENT,
						       sizeof( ENTER_COURSE_DEPARTMENT ) );

						sleep( 1 );

						int read_course_department_bytes =
						          read( clientSocket, read_buffer, 512 );
						if( read_course_department_bytes == -1 )
							perror( "Could not "
							        "read course "
							        "department." );
						read_buffer[read_course_department_bytes - 1] = '\0';
						memset( new_course.department, 0, DEPARTMENT_NAME_LENGTH );
						strcpy( new_course.department, read_buffer );
						memset( read_buffer, 0, 512 );

						// Course Credits
						write( clientSocket, ENTER_COURSE_CREDITS,
						       sizeof( ENTER_COURSE_CREDITS ) );

						sleep( 1 );

						int read_course_credits_bytes = read( clientSocket, read_buffer, 512 );
						if( read_course_credits_bytes == -1 )
							perror( "Could not "
							        "read course "
							        "credits." );
						read_buffer[read_course_credits_bytes - 1] = '\0';
						int credits                                = atoi( read_buffer );
						new_course.credits                         = credits;
						memset( read_buffer, 0, 512 );

						// Course Total No Of Seats
						write( clientSocket, ENTER_COURSE_TOTAL_SEATS,
						       sizeof( ENTER_COURSE_TOTAL_SEATS ) );

						sleep( 1 );

						int read_course_total_seats_bytes =
						          read( clientSocket, read_buffer, 512 );
						if( read_course_total_seats_bytes == -1 )
							perror( "Could not "
							        "read course "
							        "total no of "
							        "seats." );
						read_buffer[read_course_total_seats_bytes - 1] = '\0';
						int total_no_of_seats                          = atoi( read_buffer );
						new_course.total_number_of_seats               = total_no_of_seats;
						memset( read_buffer, 0, 512 );

						// Course Total No Of Available
						// Seats
						write( clientSocket, ENTER_COURSE_TOTAL_AVAILABLE_SEATS,
						       sizeof( ENTER_COURSE_TOTAL_AVAILABLE_SEATS ) );
						sleep( 1 );
						int read_course_total_no_of_available_bytes =
						          read( clientSocket, read_buffer, 512 );
						if( read_course_total_no_of_available_bytes == -1 )
							perror( "Could not "
							        "read course "
							        "total no of "
							        "available "
							        "seats." );
						read_buffer[read_course_total_no_of_available_bytes - 1] = '\0';
						int total_no_of_available_seats      = atoi( read_buffer );
						new_course.number_of_available_seats = total_no_of_available_seats;
						memset( read_buffer, 0, 512 );

						// Status
						new_course.status = 1;

						// Course Index;
						// Apply Semaphores here... OR
						// maintain the details in
						// another file...
						no_of_courses++;

						// Course ID
						memset( new_course.course_id, 0, COURSEID_LENGTH );
						snprintf( new_course.course_id, COURSEID_LENGTH, "CC%03d",
						          no_of_courses );

						// Probably I might have to add
						// semaphore here....

						// Adding the course_id to the
						// set of courses offered by the
						// faculty
						strcpy( &f_record.offering_courses[f_record.no_of_offering_courses - 1]
						                                  [COURSEID_LENGTH],
						        new_course.course_id );

						// Incrementing the number of
						// courses that a faculty wants
						// to add
						f_record.no_of_offering_courses++;

						write_faculty_record( fileDescriptor_faculty, &f_record, faculty_id,
						                      sizeof( struct faculty ), 1 );

						write( clientSocket, FACULTY_RECORD_MODIFIED,
						       sizeof( FACULTY_RECORD_MODIFIED ) );
						write( 1, FACULTY_RECORD_MODIFIED, sizeof( FACULTY_RECORD_MODIFIED ) );
						sleep( 1 );
						write( clientSocket, COURSE_CREATED_SUCCESSFULLY,
						       sizeof( COURSE_CREATED_SUCCESSFULLY ) );
						write( 1, COURSE_CREATED_SUCCESSFULLY,
						       sizeof( COURSE_CREATED_SUCCESSFULLY ) );

						write_course_record( fileDescriptor_course, &new_course, 0,
						                     sizeof( course ), 0 );
					}
					else
					{
						write( clientSocket, CANNOT_ADD_COURSES, sizeof( CANNOT_ADD_COURSES ) );
						write( 1, CANNOT_ADD_COURSES, sizeof( CANNOT_ADD_COURSES ) );
					}
				}
				else
				{
					write( clientSocket, CANNOT_ADD_COURSE_FACULTY,
					       sizeof( CANNOT_ADD_COURSE_FACULTY ) );
					write( 1, CANNOT_ADD_COURSE_FACULTY, sizeof( CANNOT_ADD_COURSE_FACULTY ) );
				}
			}
			else { write( clientSocket, FACULTY_RECORD_NOT_FOUND, sizeof( FACULTY_RECORD_NOT_FOUND ) ); }
		}
		close( fileDescriptor_faculty );
	}
	free( read_buffer );
	close( fileDescriptor_course );
}

void change_password_faculty( int clientSocket, int faculty_id )
{
	struct faculty record;
	char *password_buffer, *confirm_password_buffer;
	password_buffer         = ( char* ) malloc( PASSWORD_LENGTH * sizeof( char ) );
	confirm_password_buffer = ( char* ) malloc( PASSWORD_LENGTH * sizeof( char ) );
	memset( password_buffer, 0, PASSWORD_LENGTH );
	memset( confirm_password_buffer, 0, PASSWORD_LENGTH );

	int fileDescriptor = open( "faculty.txt", O_CREAT | O_RDWR, 0777 );
	if( fileDescriptor == -1 )
		perror( FACULTY_FILE_OPEN_FAILED );
	else
	{
		while( 1 )
		{
			write( clientSocket, ENTER_PASSWORD, sizeof( ENTER_PASSWORD ) );
			sleep( 1 );
			int read_faculty_password_bytes = read( clientSocket, password_buffer, PASSWORD_LENGTH );
			if( read_faculty_password_bytes == -1 ) perror( "Reading from socket failed. \n" );
			password_buffer[read_faculty_password_bytes - 1] = '\0';
			write( 1, password_buffer, strlen( password_buffer ) );

			write( clientSocket, ENTER_PASSWORD_CONFIRM, sizeof( ENTER_PASSWORD_CONFIRM ) );
			sleep( 1 );
			int read_faculty_password_confirm_bytes =
			          read( clientSocket, confirm_password_buffer, PASSWORD_LENGTH );
			if( read_faculty_password_confirm_bytes == -1 ) perror( "Reading from socket failed. \n" );
			confirm_password_buffer[read_faculty_password_confirm_bytes - 1] = '\0';
			write( 1, confirm_password_buffer, strlen( confirm_password_buffer ) );

			int retval =
			          read_faculty_record( fileDescriptor, &record, faculty_id, sizeof( struct faculty ) );

			if( !strcmp( password_buffer, confirm_password_buffer ) && retval == 1 )
			{
				write( clientSocket, PASSWORDS_MATCH_CLIENT, sizeof( PASSWORDS_MATCH_CLIENT ) );
				write( 1, PASSWORDS_MATCH_SERVER, sizeof( PASSWORDS_MATCH_SERVER ) );
				sleep( 1 );
				write( clientSocket, PASSWORDS_CHANGED_CLIENT, sizeof( PASSWORDS_CHANGED_CLIENT ) );
				write( 1, FACULTY_PASSWORDS_CHANGED_SERVER,
				       sizeof( FACULTY_PASSWORDS_CHANGED_SERVER ) );

				// Changing Password
				memset( record.password, 0, PASSWORD_LENGTH );
				strcpy( record.password, password_buffer );

				write_faculty_record( fileDescriptor, &record, faculty_id, sizeof( faculty ), 1 );
				break;
			}
			else
			{
				write( clientSocket, PASSWORDS_DO_NOT_MATCH, sizeof( PASSWORDS_DO_NOT_MATCH ) );
				write( 1, PASSWORDS_DO_NOT_MATCH, sizeof( PASSWORDS_DO_NOT_MATCH ) );
				sleep( 1 );
			}
		}
	}
	close( fileDescriptor );
	free( password_buffer );
	free( confirm_password_buffer );
}

void logout_and_exit_faculty() { return; }

void faculty_menu_handler( int clientSocket, int faculty_id )
{
	int condition = 1;
	while( condition )
	{
		char* read_buffer;
		read_buffer = ( char* ) malloc( 512 * sizeof( char ) );
		memset( read_buffer, 0, 512 );
		write( 1, WRITE_FACULTY_PROMPT_SUCCESSFUL, sizeof( WRITE_FACULTY_PROMPT_SUCCESSFUL ) );
		ssize_t read_bytes = read( clientSocket, read_buffer, 512 );

		if( read_bytes == -1 )
		{
			write( 2, READ_FACULTY_CHOICE_UNSUCCESSFUL, sizeof( READ_FACULTY_CHOICE_UNSUCCESSFUL ) );
		}
		else
		{
			write( 1, read_buffer, strlen( read_buffer ) );
			int userChoice = atoi( read_buffer );
			if( userChoice == 0 )
			{
				write( 1, FACULTY_NO_CHOICE_SERVER, sizeof( FACULTY_NO_CHOICE_SERVER ) );
				write( clientSocket, FACULTY_NO_CHOICE_CLIENT, sizeof( FACULTY_NO_CHOICE_CLIENT ) );
			}
			else
			{
				switch( userChoice )
				{
				case 1: add_new_course( clientSocket, faculty_id ); break;
				case 2: remove_course_from_catalog( clientSocket, faculty_id ); break;
				case 3: view_offering_courses( clientSocket, faculty_id ); break;
				case 4: update_course( clientSocket, faculty_id ); break;
				case 5: change_password_faculty( clientSocket, faculty_id ); break;
				case 6:
					logout_and_exit_faculty();
					condition = 0;
					break;
				default:
					write( clientSocket, FACULTY_INCORRECT_CHOICE_CLIENT,
					       sizeof( FACULTY_INCORRECT_CHOICE_CLIENT ) );
					write( 1, FACULTY_INCORRECT_CHOICE_SERVER,
					       sizeof( FACULTY_INCORRECT_CHOICE_SERVER ) );
					break;
				}
			}
		}
		sleep( 1 );
		if( condition == 1 ) write( clientSocket, FACULTY_MENU, sizeof( FACULTY_MENU ) );
		free( read_buffer );
	}
}

void faculty_connection_handler( int clientSocket )
{
	char *username_buffer, *password_buffer;
	struct faculty record;
	username_buffer    = ( char* ) malloc( USERNAME_LENGTH * sizeof( char ) );
	password_buffer    = ( char* ) malloc( PASSWORD_LENGTH * sizeof( char ) );
	int fileDescriptor = open( "faculty.txt", O_RDONLY, 0777 );
	if( fileDescriptor == -1 ) { perror( FACULTY_FILE_OPEN_FAILED ); }
	else
	{
		int faculty_id;
		while( 1 )
		{
			memset( username_buffer, 0, USERNAME_LENGTH );
			memset( password_buffer, 0, PASSWORD_LENGTH );
			sleep( 1 );
			write( clientSocket, LOGIN_MESSAGE, sizeof( LOGIN_MESSAGE ) );
			int read_username_bytes = read( clientSocket, username_buffer, USERNAME_LENGTH );
			username_buffer[read_username_bytes - 1] = '\0';

			char* temp_username_buffer = ( char* ) malloc( USERNAME_LENGTH * sizeof( char ) );
			memset( temp_username_buffer, 0, USERNAME_LENGTH );
			strcpy( temp_username_buffer, username_buffer + strlen( username_buffer ) - 2 );
			faculty_id = atoi( temp_username_buffer );
			free( temp_username_buffer );
			if( faculty_id == 0 )
			{
				write( clientSocket, AUTHENTICATION_USERNAME_EMPTY,
				       sizeof( AUTHENTICATION_USERNAME_EMPTY ) );
			}
			else
			{
				int retval = read_faculty_record( fileDescriptor, &record, faculty_id,
				                                  sizeof( struct faculty ) );
				if( retval == 1 )
				{

					// Comment out the below line...
					write( 1, username_buffer, strlen( username_buffer ) );

					write( clientSocket, PASSWORD_MESSAGE, sizeof( PASSWORD_MESSAGE ) );
					int read_password_bytes =
					          read( clientSocket, password_buffer, PASSWORD_LENGTH );

					write( 1, password_buffer, strlen( password_buffer ) );
					password_buffer[read_password_bytes - 1] = '\0';

					if( !( strcmp( record.password, password_buffer ) ) )
					{
						free( username_buffer );
						free( password_buffer );
						close( fileDescriptor );
						write( clientSocket, AUTHENTICATION_SUCCESS,
						       sizeof( AUTHENTICATION_SUCCESS ) );
						sleep( 1 );
						write( clientSocket, FACULTY_MENU, sizeof( FACULTY_MENU ) );
						break;
					}
					else
					{
						write( clientSocket, AUTHENTICATION_FAILED,
						       sizeof( AUTHENTICATION_FAILED ) );
					}
				}
				else
				{
					write( clientSocket, FACULTY_RECORD_NOT_FOUND,
					       sizeof( FACULTY_RECORD_NOT_FOUND ) );
				}
			}
		}
		faculty_menu_handler( clientSocket, faculty_id );
	}
}