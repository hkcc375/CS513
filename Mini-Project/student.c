#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "common.h"
#include "course.h"
#include "course_student_mapping.h"
#include "server_constants.h"
#include "socket_constants.h"
#include "student.h"

void drop_course( int clientSocket, int student_id ) {}
void view_enrolled_courses( int clientSocket, int student_id ) {}

void enroll_course( int clientSocket, int student_id )
{
	struct student student_record;
	struct course course_record;
	struct mapping mapping_record;
	int course_id;
	char* read_buffer = ( char* ) malloc( 512 * sizeof( char ) );
	memset( read_buffer, 0, 512 );

	int fileDescriptor_mapping = open( "mapping.txt", O_CREAT | O_WRONLY, 0777 );
	if( fileDescriptor_mapping == -1 ) perror( MAPPING_FILE_OPEN_FAILED );

	int fileDescriptor_student = open( "student.txt", O_CREAT | O_RDWR, 0777 );
	if( fileDescriptor_student == -1 ) perror( STUDENT_FILE_OPEN_FAILED );

	int fileDescriptor_course = open( "course.txt", O_CREAT | O_RDWR, 0777 );
	if( fileDescriptor_student == -1 ) perror( COURSE_FILE_OPEN_FAILED );

	write( clientSocket, ENTER_COURSE_ID, sizeof( ENTER_COURSE_ID ) );
	sleep( 1 );
	int read_course_id_bytes = read( clientSocket, read_buffer, 512 );
	if( read_course_id_bytes == -1 ) perror( "Could not read course id." );
	read_buffer[read_course_id_bytes - 1] = '\0';

	char* temp_course_id_buffer = ( char* ) malloc( COURSEID_LENGTH * sizeof( char ) );
	memset( temp_course_id_buffer, 0, COURSEID_LENGTH );
	strcpy( temp_course_id_buffer, read_buffer + strlen( read_buffer ) - 3 );
	course_id = atoi( temp_course_id_buffer );
	// free( temp_course_id_buffer );
	if( course_id == 0 ) { write( clientSocket, ENROLL_COURSEID_EMPTY, sizeof( ENROLL_COURSEID_EMPTY ) ); }
	else
	{
		int retval_student = read_student_record( fileDescriptor_student, &student_record, student_id,
		                                          sizeof( struct student ) );

		int retval_course = read_course_record( fileDescriptor_course, &course_record, course_id,
		                                        sizeof( struct course ) );
		if( retval_student == 1 )
		{
			if( student_record.number_of_courses_taken < MAX_COURSES_PER_STUDENT )
			{
				int flag = 1;
				// This means that the student has taken
				// less number of courses than the
				// MAX_COURSES_PER_STUDENT
				for( int i = 0; i < student_record.number_of_courses_taken; i++ )
				{
					if( !strcmp( student_record.enrolled_courses[i], temp_course_id_buffer ) )
					{
						flag = 0;
						// This means that
						// the student has
						// already enrolled
						// to this course
						write( clientSocket, COURSE_ALREADY_ENROLLED_CLIENT,
						       sizeof( COURSE_ALREADY_ENROLLED_CLIENT ) );
						write( 1, COURSE_ALREADY_ENROLLED_SERVER,
						       sizeof( COURSE_ALREADY_ENROLLED_SERVER ) );
						break;
					}
				}
				if( flag == 1 )
				{
					// This means that the student
					// has not enrolled to this
					// course && he has taken no.
					// of courses less than
					// MAX_COURSES_PER_STUDENT
					if( retval_course == 1 )
					{
						if( course_record.number_of_available_seats == 0 )
						{
							write( clientSocket,
							       CANNOT_ENROLL_COURSE_MAX_LIMIT_REACHED_STUDENT,
							       sizeof( CANNOT_ENROLL_COURSE_MAX_LIMIT_REACHED_STUDENT ) );
						}
						else
						{
							// We need
							// to
							// modify
							// the
							// course
							// details
							// such as
							// no. of
							// available
							// seats
							// Write
							// the
							// structure
							// to the
							// mapping
							// file.

							// Maybe I
							// have to
							// apply
							// semaphore..
							course_record.number_of_available_seats -= 1;

							write_course_record( fileDescriptor_course, &course_record,
							                     course_id, sizeof( course ), 1 );
							write( 1, COURSE_RECORD_MODIFIED,
							       sizeof( COURSE_RECORD_MODIFIED ) );

							// Maybe I may have to use semaphore..
							mapping_record.course_id  = course_id;
							mapping_record.student_id = student_id;
							mapping_record.status     = 1;
							write_mapping_record( fileDescriptor_mapping, &mapping_record,
							                      0, sizeof( struct mapping ), 0 );

							// Modifying student enrolled course details ..
							strcpy( &student_record.enrolled_courses
							                   [student_record.number_of_courses_taken - 1]
							                   [COURSEID_LENGTH],
							        course_record.course_id );
							write_student_record( fileDescriptor_student, &student_record,
							                      student_id, sizeof( struct student ), 1 );
							write( 1, STUDENT_RECORD_MODIFIED,
							       sizeof( STUDENT_RECORD_MODIFIED ) );
							sleep( 1 );
							write( clientSocket, ENROLL_COURSE_SUCCESSFUL_CLIENT,
							       sizeof( ENROLL_COURSE_SUCCESSFUL_CLIENT ) );

							write( 1, ENROLL_COURSE_SUCCESSFUL_SERVER,
							       sizeof( ENROLL_COURSE_SUCCESSFUL_SERVER ) );
						}
					}
					else
					{
						// Course record was
						// not found..
						write( clientSocket, COURSE_RECORD_NOT_FOUND,
						       sizeof( COURSE_RECORD_NOT_FOUND ) );
					}
				}
			}
			else
			{
				// Student has enrolled for
				// MAX_COURSES_PER_STUDENT. He cant
				// enroll for more courses.
				write( clientSocket, CANNOT_ENROLL_COURSE, sizeof( CANNOT_ENROLL_COURSE ) );
			}
		}
		else
		{
			// This means that the student record does not
			// exist.
			write( clientSocket, STUDENT_RECORD_NOT_FOUND, sizeof( STUDENT_RECORD_NOT_FOUND ) );
		}
	}
	free( temp_course_id_buffer );
	close( fileDescriptor_course );
	close( fileDescriptor_student );
	close( fileDescriptor_mapping );
	free( read_buffer );
}

void view_all_courses( int clientSocket, int student_id )
{
	struct course record;
	char* read_buffer = ( char* ) malloc( 512 * sizeof( char ) );
	memset( read_buffer, 0, 512 );
	int fileDescriptor = open( "course.txt", O_CREAT | O_RDONLY, 0777 );
	if( fileDescriptor == -1 )
		perror( COURSE_FILE_OPEN_FAILED );
	else
	{
		int bytesToRead;
		while( ( bytesToRead = read( fileDescriptor, &record, sizeof( struct course ) ) ) > 0 )
		{
			// We have reached the EOF
			if( bytesToRead != sizeof( struct course ) ) { perror( COURSE_RECORD_EOF ); }
			else
			{
				// Course Name
				strcat( read_buffer, "Course Name : " );
				strcat( read_buffer, record.course_name );
				strcat( read_buffer, "\n" );
				write( clientSocket, read_buffer, strlen( read_buffer ) );
				memset( read_buffer, 0, 512 );

				// Course ID
				strcat( read_buffer, "Course ID : " );
				strcat( read_buffer, record.course_id );
				strcat( read_buffer, "\n" );
				write( clientSocket, read_buffer, strlen( read_buffer ) );
				memset( read_buffer, 0, 512 );

				// Course Department
				strcat( read_buffer, "Department : " );
				strcat( read_buffer, record.department );
				strcat( read_buffer, "\n" );
				write( clientSocket, read_buffer, strlen( read_buffer ) );
				memset( read_buffer, 0, 512 );

				// Course Credits
				strcat( read_buffer, "Credits : " );
				char* temp_buffer_credits = ( char* ) malloc( 10 * sizeof( char ) );
				snprintf( temp_buffer_credits, 10, "%d", record.credits );
				strcat( read_buffer, temp_buffer_credits );
				strcat( read_buffer, "\n" );
				write( clientSocket, read_buffer, strlen( read_buffer ) );
				free( temp_buffer_credits );
				memset( read_buffer, 0, 512 );

				// Course Total No. Of Seats
				strcat( read_buffer, "Total No. Of Seats : " );
				char* temp_buffer_total_seats = ( char* ) malloc( 10 * sizeof( char ) );
				snprintf( temp_buffer_total_seats, 10, "%d", record.total_number_of_seats );
				strcat( read_buffer, temp_buffer_total_seats );
				strcat( read_buffer, "\n" );
				write( clientSocket, read_buffer, strlen( read_buffer ) );
				free( temp_buffer_total_seats );
				memset( read_buffer, 0, 512 );

				// Course Total No. Of Available Seats
				strcat( read_buffer, "Total No. Of Available Seats : " );
				char* temp_buffer_total_seats_available = ( char* ) malloc( 10 * sizeof( char ) );
				snprintf( temp_buffer_total_seats_available, 10, "%d",
				          record.number_of_available_seats );
				strcat( read_buffer, temp_buffer_total_seats_available );
				strcat( read_buffer, "\n" );
				strcat( read_buffer, "\n" );
				write( clientSocket, read_buffer, strlen( read_buffer ) );
				free( temp_buffer_total_seats_available );
				memset( read_buffer, 0, 512 );
			}
		}
	}
	free( read_buffer );
	close( fileDescriptor );
}

void change_password_student( int clientSocket, int student_id )
{
	struct student record;
	char *password_buffer, *confirm_password_buffer;
	password_buffer         = ( char* ) malloc( PASSWORD_LENGTH * sizeof( char ) );
	confirm_password_buffer = ( char* ) malloc( PASSWORD_LENGTH * sizeof( char ) );
	memset( password_buffer, 0, PASSWORD_LENGTH );
	memset( confirm_password_buffer, 0, PASSWORD_LENGTH );

	int fileDescriptor = open( "student.txt", O_CREAT | O_RDWR, 0777 );
	if( fileDescriptor == -1 )
		perror( STUDENT_FILE_OPEN_FAILED );
	else
	{
		while( 1 )
		{
			write( clientSocket, ENTER_PASSWORD, sizeof( ENTER_PASSWORD ) );
			sleep( 1 );
			int read_student_password_bytes = read( clientSocket, password_buffer, PASSWORD_LENGTH );
			if( read_student_password_bytes == -1 ) perror( "Reading from socket failed. \n" );
			password_buffer[read_student_password_bytes - 1] = '\0';
			write( 1, password_buffer, strlen( password_buffer ) );

			write( clientSocket, ENTER_PASSWORD_CONFIRM, sizeof( ENTER_PASSWORD_CONFIRM ) );
			sleep( 1 );
			int read_student_password_confirm_bytes =
			          read( clientSocket, confirm_password_buffer, PASSWORD_LENGTH );
			if( read_student_password_confirm_bytes == -1 ) perror( "Reading from socket failed. \n" );
			confirm_password_buffer[read_student_password_confirm_bytes - 1] = '\0';
			write( 1, confirm_password_buffer, strlen( confirm_password_buffer ) );

			int retval =
			          read_student_record( fileDescriptor, &record, student_id, sizeof( struct student ) );

			if( !strcmp( password_buffer, confirm_password_buffer ) && retval == 1 )
			{
				write( clientSocket, PASSWORDS_MATCH_CLIENT, sizeof( PASSWORDS_MATCH_CLIENT ) );
				write( 1, PASSWORDS_MATCH_SERVER, sizeof( PASSWORDS_MATCH_SERVER ) );
				sleep( 1 );
				write( clientSocket, PASSWORDS_CHANGED_CLIENT, sizeof( PASSWORDS_CHANGED_CLIENT ) );
				write( 1, STUDENT_PASSWORDS_CHANGED_SERVER,
				       sizeof( STUDENT_PASSWORDS_CHANGED_SERVER ) );

				// Changing Password
				memset( record.password, 0, PASSWORD_LENGTH );
				strcpy( record.password, password_buffer );

				write_student_record( fileDescriptor, &record, student_id, sizeof( student ), 1 );
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

void student_menu_handler( int clientSocket, int student_id )
{
	int condition = 1;
	while( condition )
	{
		char* read_buffer;
		read_buffer = ( char* ) malloc( 512 * sizeof( char ) );
		memset( read_buffer, 0, 512 );
		write( 1, WRITE_STUDENT_PROMPT_SUCCESSFUL, sizeof( WRITE_STUDENT_PROMPT_SUCCESSFUL ) );
		ssize_t read_bytes = read( clientSocket, read_buffer, 512 );

		if( read_bytes == -1 )
		{
			write( 2, READ_STUDENT_CHOICE_UNSUCCESSFUL, sizeof( READ_STUDENT_CHOICE_UNSUCCESSFUL ) );
		}
		else
		{
			write( 1, read_buffer, strlen( read_buffer ) );
			int userChoice = atoi( read_buffer );
			if( userChoice == 0 )
			{
				write( 1, STUDENT_NO_CHOICE_SERVER, sizeof( STUDENT_NO_CHOICE_SERVER ) );
				write( clientSocket, STUDENT_NO_CHOICE_CLIENT, sizeof( STUDENT_NO_CHOICE_CLIENT ) );
			}
			else
			{
				switch( userChoice )
				{
				case 1: enroll_course( clientSocket, student_id ); break;
				case 2: drop_course( clientSocket, student_id ); break;
				case 3: view_all_courses( clientSocket, student_id ); break;
				case 4: view_enrolled_courses( clientSocket, student_id ); break;
				case 5: change_password_student( clientSocket, student_id ); break;
				case 6:
					logout_and_exit_student();
					condition = 0;
					break;
				default:
					write( clientSocket, STUDENT_INCORRECT_CHOICE_CLIENT,
					       sizeof( STUDENT_INCORRECT_CHOICE_CLIENT ) );
					write( 1, STUDENT_INCORRECT_CHOICE_SERVER,
					       sizeof( STUDENT_INCORRECT_CHOICE_SERVER ) );
					break;
				}
			}
		}
		sleep( 1 );
		if( condition == 1 ) write( clientSocket, STUDENT_MENU, sizeof( STUDENT_MENU ) );
		free( read_buffer );
	}
}

void logout_and_exit_student() { return; }

void student_connection_handler( int clientSocket )
{
	char *username_buffer, *password_buffer;
	struct student record;
	username_buffer    = ( char* ) malloc( USERNAME_LENGTH * sizeof( char ) );
	password_buffer    = ( char* ) malloc( PASSWORD_LENGTH * sizeof( char ) );
	int fileDescriptor = open( "student.txt", O_RDONLY, 0777 );
	if( fileDescriptor == -1 ) { perror( STUDENT_FILE_OPEN_FAILED ); }
	else
	{
		int student_id;
		int active = 1;
		while( active )
		{
			memset( username_buffer, 0, USERNAME_LENGTH );
			memset( password_buffer, 0, PASSWORD_LENGTH );
			sleep( 1 );
			write( clientSocket, LOGIN_MESSAGE, sizeof( LOGIN_MESSAGE ) );
			int read_username_bytes = read( clientSocket, username_buffer, USERNAME_LENGTH );
			username_buffer[read_username_bytes - 1] = '\0';

			char* temp_username_buffer = ( char* ) malloc( USERNAME_LENGTH * sizeof( char ) );
			memset( temp_username_buffer, 0, USERNAME_LENGTH );
			strcpy( temp_username_buffer, username_buffer + strlen( username_buffer ) - 4 );
			student_id = atoi( temp_username_buffer );
			free( temp_username_buffer );
			if( student_id == 0 )
			{
				write( clientSocket, AUTHENTICATION_USERNAME_EMPTY,
				       sizeof( AUTHENTICATION_USERNAME_EMPTY ) );
			}
			else
			{

				int retval = read_student_record( fileDescriptor, &record, student_id,
				                                  sizeof( struct student ) );
				if( retval == 1 )
				{
					if( record.status == 1 )
					{
						// Comment out the
						// below line...
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
							write( clientSocket, STUDENT_MENU, sizeof( STUDENT_MENU ) );
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
						active = 0;
						write( clientSocket, STUDENT_INACTIVE_LOGIN,
						       sizeof( STUDENT_INACTIVE_LOGIN ) );
						write( 1, STUDENT_INACTIVE, sizeof( STUDENT_INACTIVE ) );
					}
				}
				else
				{
					write( clientSocket, STUDENT_RECORD_NOT_FOUND,
					       sizeof( STUDENT_RECORD_NOT_FOUND ) );
				}
			}
		}
		if( active == 0 )
			logout_and_exit_student();
		else
			student_menu_handler( clientSocket, student_id );
	}
}