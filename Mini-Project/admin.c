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

void modify_faculty_details( int clientSocket )
{
	struct faculty record;
	char* read_buffer;
	read_buffer = ( char* ) malloc( 512 * sizeof( char ) );
	memset( read_buffer, 0, 512 );

	int fileDescriptor = open( "faculty.txt", O_CREAT | O_RDWR, 0777 );
	if( fileDescriptor == -1 )
		perror( FACULTY_FILE_OPEN_FAILED );
	else
	{
		write( clientSocket, ENTER_FACULTY_ID, sizeof( ENTER_FACULTY_ID ) );
		sleep( 1 );
		int read_faculty_id_bytes = read( clientSocket, read_buffer, 512 );
		if( read_faculty_id_bytes == -1 ) perror( "Reading from socket failed. \n" );
		read_buffer[read_faculty_id_bytes - 1] = '\0';
		int faculty_id                         = atoi( read_buffer );
		if( faculty_id == 0 )
		{
			write( 1, FACULTY_RECORD_INVALID_SERVER, sizeof( FACULTY_RECORD_INVALID_SERVER ) );
			write( clientSocket, FACULTY_RECORD_INVALID_CLIENT, sizeof( FACULTY_RECORD_INVALID_CLIENT ) );
		}
		else
		{
			int retval =
			          read_faculty_record( fileDescriptor, &record, faculty_id, sizeof( struct faculty ) );
			if( retval == 1 )
			{
				memset( read_buffer, 0, 512 );

				// Name
				write( clientSocket, ENTER_FACULTY_NAME, sizeof( ENTER_FACULTY_NAME ) );
				sleep( 1 );
				int read_name_bytes = read( clientSocket, read_buffer, 512 );
				if( read_name_bytes == -1 ) perror( "Could not read faculty name." );
				read_buffer[read_name_bytes - 1] = '\0';
				memset( record.name, 0, NAME_LENGTH );
				strcpy( record.name, read_buffer );
				memset( read_buffer, 0, 512 );

				// Department
				write( clientSocket, ENTER_FACULTY_DEPARTMENT, sizeof( ENTER_FACULTY_DEPARTMENT ) );
				sleep( 1 );
				int read_department_bytes = read( clientSocket, read_buffer, 512 );
				if( read_department_bytes == -1 )
					perror( "Could not read faculty "
					        "department." );
				read_buffer[read_department_bytes - 1] = '\0';
				memset( record.department, 0, DEPARTMENT_NAME_LENGTH );
				strcpy( record.department, read_buffer );
				memset( read_buffer, 0, 512 );

				// Designation
				write( clientSocket, ENTER_FACULTY_DESIGNATION, sizeof( ENTER_FACULTY_DESIGNATION ) );
				sleep( 1 );
				int read_designation_bytes = read( clientSocket, read_buffer, 512 );
				if( read_designation_bytes == -1 )
					perror( "Could not read faculty "
					        "designation." );
				read_buffer[read_designation_bytes - 1] = '\0';
				memset( record.designation, 0, DESIGNATION_LENGTH );
				strcpy( record.designation, read_buffer );
				memset( read_buffer, 0, 512 );

				// Email
				write( clientSocket, ENTER_FACULTY_EMAIL, sizeof( ENTER_FACULTY_EMAIL ) );
				sleep( 1 );
				int read_email_bytes = read( clientSocket, read_buffer, 512 );
				if( read_email_bytes == -1 ) perror( "Could not read faculty email." );
				read_buffer[read_email_bytes - 1] = '\0';
				memset( record.email, 0, EMAIL_LENGTH );
				strcpy( record.email, read_buffer );
				memset( read_buffer, 0, 512 );

				// Address
				write( clientSocket, ENTER_FACULTY_ADDRESS, sizeof( ENTER_FACULTY_ADDRESS ) );
				sleep( 1 );
				int read_address_bytes = read( clientSocket, read_buffer, 512 );
				if( read_address_bytes == -1 )
					perror( "Could not read faculty "
					        "addsress." );
				read_buffer[read_address_bytes - 1] = '\0';
				memset( record.address, 0, ADDRESS_LENGTH );
				strcpy( record.address, read_buffer );
				memset( read_buffer, 0, 512 );

				write_faculty_record( fileDescriptor, &record, faculty_id, sizeof( faculty ), 1 );
				write( clientSocket, FACULTY_RECORD_MODIFIED, sizeof( FACULTY_RECORD_MODIFIED ) );
			}
			else { write( clientSocket, FACULTY_RECORD_NOT_FOUND, sizeof( FACULTY_RECORD_NOT_FOUND ) ); }
		}
	}
	close( fileDescriptor );
	free( read_buffer );
}

void view_faculty_details( int clientSocket )
{
	struct faculty record;
	char* read_buffer;
	read_buffer = ( char* ) malloc( 512 * sizeof( char ) );
	memset( read_buffer, 0, 512 );

	int fileDescriptor = open( "faculty.txt", O_CREAT | O_RDONLY, 0777 );
	if( fileDescriptor == -1 )
		perror( FACULTY_FILE_OPEN_FAILED );
	else
	{
		write( clientSocket, ENTER_FACULTY_ID, sizeof( ENTER_FACULTY_ID ) );
		sleep( 1 );
		int read_faculty_id_bytes = read( clientSocket, read_buffer, 512 );
		if( read_faculty_id_bytes == -1 ) perror( "Reading from socket failed. \n" );
		read_buffer[read_faculty_id_bytes - 1] = '\0';
		int faculty_id                         = atoi( read_buffer );
		if( faculty_id == 0 )
		{
			write( 1, FACULTY_RECORD_INVALID_SERVER, sizeof( FACULTY_RECORD_INVALID_SERVER ) );
			write( clientSocket, FACULTY_RECORD_INVALID_CLIENT, sizeof( FACULTY_RECORD_INVALID_CLIENT ) );
		}
		else
		{
			int retval =
			          read_faculty_record( fileDescriptor, &record, faculty_id, sizeof( struct faculty ) );
			if( retval == 1 )
			{
				memset( read_buffer, 0, 512 );

				// Name
				strcat( read_buffer, "Name : " );
				strcat( read_buffer, record.name );
				strcat( read_buffer, "\n" );
				write( clientSocket, read_buffer, strlen( read_buffer ) );
				memset( read_buffer, 0, 512 );

				// Department
				strcat( read_buffer, "Department : " );
				strcat( read_buffer, record.department );
				strcat( read_buffer, "\n" );
				write( clientSocket, read_buffer, strlen( read_buffer ) );
				memset( read_buffer, 0, 512 );

				// Designation
				strcat( read_buffer, "Designation : " );
				strcat( read_buffer, record.designation );
				strcat( read_buffer, "\n" );
				write( clientSocket, read_buffer, strlen( read_buffer ) );
				memset( read_buffer, 0, 512 );

				// Email
				strcat( read_buffer, "Email : " );
				strcat( read_buffer, record.email );
				strcat( read_buffer, "\n" );
				write( clientSocket, read_buffer, strlen( read_buffer ) );
				memset( read_buffer, 0, 512 );

				// Address
				strcat( read_buffer, "Address : " );
				strcat( read_buffer, record.address );
				strcat( read_buffer, "\n" );
				write( clientSocket, read_buffer, strlen( read_buffer ) );
				memset( read_buffer, 0, 512 );

				// Username
				strcat( read_buffer, "Login id / Username : " );
				strcat( read_buffer, record.username );
				strcat( read_buffer, "\n" );
				write( clientSocket, read_buffer, strlen( read_buffer ) );
				memset( read_buffer, 0, 512 );
			}
			else { write( clientSocket, FACULTY_RECORD_NOT_FOUND, sizeof( FACULTY_RECORD_NOT_FOUND ) ); }
		}
	}
	close( fileDescriptor );
	free( read_buffer );
}

void add_faculty( int clientSocket )
{
	char* read_buffer;
	read_buffer = ( char* ) malloc( 512 * sizeof( char ) );
	memset( read_buffer, 0, 512 );

	int fileDescriptor = open( "faculty.txt", O_CREAT | O_WRONLY, 0777 );
	if( fileDescriptor == -1 )
		perror( FACULTY_FILE_OPEN_FAILED );
	else
	{
		if( no_of_faculty < MAX_FACULTY )
		{
			struct faculty new_faculty;

			// Name
			write( clientSocket, ENTER_FACULTY_NAME, sizeof( ENTER_FACULTY_NAME ) );
			sleep( 1 );
			int read_name_bytes = read( clientSocket, read_buffer, 512 );
			if( read_name_bytes == -1 ) perror( "Could not read faculty name." );
			read_buffer[read_name_bytes - 1] = '\0';
			memset( new_faculty.name, 0, NAME_LENGTH );
			strcpy( new_faculty.name, read_buffer );
			memset( read_buffer, 0, 512 );

			// Department
			write( clientSocket, ENTER_FACULTY_DEPARTMENT, sizeof( ENTER_FACULTY_DEPARTMENT ) );
			sleep( 1 );
			int read_department_bytes = read( clientSocket, read_buffer, 512 );
			if( read_department_bytes == -1 ) perror( "Could not read faculty department." );
			read_buffer[read_department_bytes - 1] = '\0';
			memset( new_faculty.department, 0, DEPARTMENT_NAME_LENGTH );
			strcpy( new_faculty.department, read_buffer );
			memset( read_buffer, 0, 512 );

			// Designation
			write( clientSocket, ENTER_FACULTY_DESIGNATION, sizeof( ENTER_FACULTY_DESIGNATION ) );
			sleep( 1 );
			int read_designation_bytes = read( clientSocket, read_buffer, 512 );
			if( read_designation_bytes == -1 ) perror( "Could not read faculty designation." );
			read_buffer[read_designation_bytes - 1] = '\0';
			memset( new_faculty.designation, 0, DESIGNATION_LENGTH );
			strcpy( new_faculty.designation, read_buffer );
			memset( read_buffer, 0, 512 );

			// Email
			write( clientSocket, ENTER_FACULTY_EMAIL, sizeof( ENTER_FACULTY_EMAIL ) );
			sleep( 1 );
			int read_email_bytes = read( clientSocket, read_buffer, 512 );
			if( read_email_bytes == -1 ) perror( "Could not read faculty email." );
			read_buffer[read_email_bytes - 1] = '\0';
			memset( new_faculty.email, 0, EMAIL_LENGTH );
			strcpy( new_faculty.email, read_buffer );
			memset( read_buffer, 0, 512 );

			// Address
			write( clientSocket, ENTER_FACULTY_ADDRESS, sizeof( ENTER_FACULTY_ADDRESS ) );
			sleep( 1 );
			int read_address_bytes = read( clientSocket, read_buffer, 512 );
			if( read_address_bytes == -1 ) perror( "Could not read faculty address." );
			read_buffer[read_address_bytes - 1] = '\0';
			memset( new_faculty.address, 0, ADDRESS_LENGTH );
			strcpy( new_faculty.address, read_buffer );
			memset( read_buffer, 0, 512 );

			// Memset the fields for course
			memset( new_faculty.offering_courses, 0, sizeof( new_faculty.offering_courses ) );

			// Initialize the field for no_of_offering_courses
			new_faculty.no_of_offering_courses = 0;

			// Password
			memset( new_faculty.password, 0, PASSWORD_LENGTH );
			strcpy( new_faculty.password, "faculty" );

			// Faculty ID
			// Apply Semaphores here... OR maintain the details in
			// another file...
			no_of_faculty++;

			// Username
			memset( new_faculty.username, 0, USERNAME_LENGTH );
			snprintf( new_faculty.username, USERNAME_LENGTH, "FY%02d", no_of_faculty );

			write( clientSocket, FACULTY_CREATED_SUCCESSFULLY, sizeof( FACULTY_CREATED_SUCCESSFULLY ) );

			write_faculty_record( fileDescriptor, &new_faculty, 0, sizeof( faculty ), 0 );
		}
		else { write( clientSocket, CANNOT_ADD_FACULTY, sizeof( CANNOT_ADD_FACULTY ) ); }
	}
	free( read_buffer );
	close( fileDescriptor );
}

void logout_and_exit_admin() { return; }

void modify_student_details( int clientSocket )
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
		write( clientSocket, ENTER_STUDENT_ID, sizeof( ENTER_STUDENT_ID ) );
		sleep( 1 );
		int read_student_id_bytes = read( clientSocket, read_buffer, 512 );
		if( read_student_id_bytes == -1 ) perror( "Reading from socket failed. \n" );
		read_buffer[read_student_id_bytes - 1] = '\0';
		int student_id                         = atoi( read_buffer );
		if( student_id == 0 )
		{
			write( 1, STUDENT_RECORD_INVALID_SERVER, sizeof( STUDENT_RECORD_INVALID_SERVER ) );
			write( clientSocket, STUDENT_RECORD_INVALID_CLIENT, sizeof( STUDENT_RECORD_INVALID_CLIENT ) );
		}
		else
		{
			int retval =
			          read_student_record( fileDescriptor, &record, student_id, sizeof( struct student ) );
			if( retval == 1 )
			{
				memset( read_buffer, 0, 512 );

				// Name
				write( clientSocket, ENTER_STUDENT_NAME, sizeof( ENTER_STUDENT_NAME ) );
				sleep( 1 );
				int read_name_bytes = read( clientSocket, read_buffer, 512 );
				if( read_name_bytes == -1 ) perror( "Could not read student name." );
				read_buffer[read_name_bytes - 1] = '\0';
				memset( record.name, 0, NAME_LENGTH );
				strcpy( record.name, read_buffer );
				// printf( "%s\n", new_student.name );
				memset( read_buffer, 0, 512 );

				// Age
				write( clientSocket, ENTER_STUDENT_AGE, sizeof( ENTER_STUDENT_AGE ) );
				sleep( 1 );
				int read_age_bytes = read( clientSocket, read_buffer, 512 );
				if( read_age_bytes == -1 ) perror( "Could not read student age." );
				read_buffer[read_age_bytes - 1] = '\0';
				int age                         = atoi( read_buffer );
				record.age                      = age;
				memset( read_buffer, 0, 512 );

				// Email
				write( clientSocket, ENTER_STUDENT_EMAIL, sizeof( ENTER_STUDENT_EMAIL ) );
				sleep( 1 );
				int read_email_bytes = read( clientSocket, read_buffer, 512 );
				if( read_email_bytes == -1 ) perror( "Could not read student email." );
				read_buffer[read_email_bytes - 1] = '\0';
				memset( record.email, 0, EMAIL_LENGTH );
				strcpy( record.email, read_buffer );
				memset( read_buffer, 0, 512 );

				// Address
				write( clientSocket, ENTER_STUDENT_ADDRESS, sizeof( ENTER_STUDENT_ADDRESS ) );
				sleep( 1 );
				int read_address_bytes = read( clientSocket, read_buffer, 512 );
				if( read_address_bytes == -1 )
					perror( "Could not read student "
					        "addsress." );
				read_buffer[read_address_bytes - 1] = '\0';
				memset( record.address, 0, ADDRESS_LENGTH );
				strcpy( record.address, read_buffer );
				memset( read_buffer, 0, 512 );

				write_student_record( fileDescriptor, &record, student_id, sizeof( student ), 1 );
				write( clientSocket, STUDENT_RECORD_MODIFIED, sizeof( STUDENT_RECORD_MODIFIED ) );
			}
			else { write( clientSocket, STUDENT_RECORD_NOT_FOUND, sizeof( STUDENT_RECORD_NOT_FOUND ) ); }
		}
	}
	close( fileDescriptor );
	free( read_buffer );
}

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
		write( clientSocket, ENTER_STUDENT_ID, sizeof( ENTER_STUDENT_ID ) );
		sleep( 1 );
		int read_student_id_bytes = read( clientSocket, read_buffer, 512 );
		if( read_student_id_bytes == -1 ) perror( "Reading from socket failed. \n" );
		read_buffer[read_student_id_bytes - 1] = '\0';
		int student_id                         = atoi( read_buffer );
		if( student_id == 0 )
		{
			write( 1, STUDENT_RECORD_INVALID_SERVER, sizeof( STUDENT_RECORD_INVALID_SERVER ) );
			write( clientSocket, STUDENT_RECORD_INVALID_CLIENT, sizeof( STUDENT_RECORD_INVALID_CLIENT ) );
		}
		else
		{
			int retval =
			          read_student_record( fileDescriptor, &record, student_id, sizeof( struct student ) );
			if( retval == 1 )
			{
				memset( read_buffer, 0, 512 );

				// Probably, we might have to apply semaphore
				// here.
				record.status = 1;

				write_student_record( fileDescriptor, &record, student_id, sizeof( student ), 1 );
				write( clientSocket, STUDENT_RECORD_ACTIVATED, sizeof( STUDENT_RECORD_ACTIVATED ) );
			}
			else { write( clientSocket, STUDENT_RECORD_NOT_FOUND, sizeof( STUDENT_RECORD_NOT_FOUND ) ); }
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
		write( clientSocket, ENTER_STUDENT_ID, sizeof( ENTER_STUDENT_ID ) );
		sleep( 1 );
		int read_student_id_bytes = read( clientSocket, read_buffer, 512 );
		if( read_student_id_bytes == -1 ) perror( "Reading from socket failed. \n" );
		read_buffer[read_student_id_bytes - 1] = '\0';
		int student_id                         = atoi( read_buffer );
		if( student_id == 0 )
		{
			write( 1, STUDENT_RECORD_INVALID_SERVER, sizeof( STUDENT_RECORD_INVALID_SERVER ) );
			write( clientSocket, STUDENT_RECORD_INVALID_CLIENT, sizeof( STUDENT_RECORD_INVALID_CLIENT ) );
		}
		else
		{
			int retval =
			          read_student_record( fileDescriptor, &record, student_id, sizeof( struct student ) );
			if( retval == 1 )
			{
				memset( read_buffer, 0, 512 );

				// Probably, we might have to apply semaphore
				// here.
				record.status = 0;

				write_student_record( fileDescriptor, &record, student_id, sizeof( student ), 1 );
				write( clientSocket, STUDENT_RECORD_BLOCKED, sizeof( STUDENT_RECORD_BLOCKED ) );
			}
			else { write( clientSocket, STUDENT_RECORD_NOT_FOUND, sizeof( STUDENT_RECORD_NOT_FOUND ) ); }
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
		write( clientSocket, ENTER_STUDENT_ID, sizeof( ENTER_STUDENT_ID ) );
		sleep( 1 );
		int read_student_id_bytes = read( clientSocket, read_buffer, 512 );
		if( read_student_id_bytes == -1 ) perror( "Reading from socket failed. \n" );
		read_buffer[read_student_id_bytes - 1] = '\0';
		int student_id                         = atoi( read_buffer );
		if( student_id == 0 )
		{
			// write message on the server screen...
			write( 1, STUDENT_RECORD_INVALID_SERVER, sizeof( STUDENT_RECORD_INVALID_SERVER ) );
			write( clientSocket, STUDENT_RECORD_INVALID_CLIENT, sizeof( STUDENT_RECORD_INVALID_CLIENT ) );
		}
		else
		{
			int retval =
			          read_student_record( fileDescriptor, &record, student_id, sizeof( struct student ) );
			if( retval == 1 )
			{
				memset( read_buffer, 0, 512 );
				// Name
				strcat( read_buffer, "Name : " );
				strcat( read_buffer, record.name );
				strcat( read_buffer, "\n" );
				write( clientSocket, read_buffer, strlen( read_buffer ) );
				memset( read_buffer, 0, 512 );

				// Age
				strcat( read_buffer, "Age : " );
				char* age = ( char* ) malloc( 3 * sizeof( char ) );
				snprintf( age, 3, "%d", record.age );
				strcat( read_buffer, age );
				strcat( read_buffer, "\n" );
				write( clientSocket, read_buffer, strlen( read_buffer ) );
				memset( read_buffer, 0, 512 );
				free( age );

				// Email
				strcat( read_buffer, "Email : " );
				strcat( read_buffer, record.email );
				strcat( read_buffer, "\n" );
				write( clientSocket, read_buffer, strlen( read_buffer ) );
				memset( read_buffer, 0, 512 );

				// Address
				strcat( read_buffer, "Address : " );
				strcat( read_buffer, record.address );
				strcat( read_buffer, "\n" );
				write( clientSocket, read_buffer, strlen( read_buffer ) );
				memset( read_buffer, 0, 512 );

				// Username
				strcat( read_buffer, "Login id / Username : " );
				strcat( read_buffer, record.username );
				strcat( read_buffer, "\n" );
				write( clientSocket, read_buffer, strlen( read_buffer ) );
				memset( read_buffer, 0, 512 );

				// Status
				if( record.status == 1 )
					strcat( read_buffer, STUDENT_ACTIVE );
				else
					strcat( read_buffer, STUDENT_INACTIVE );
				write( clientSocket, read_buffer, strlen( read_buffer ) );
				memset( read_buffer, 0, 512 );
			}
			else { write( clientSocket, STUDENT_RECORD_NOT_FOUND, sizeof( STUDENT_RECORD_NOT_FOUND ) ); }
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

	int fileDescriptor = open( "student.txt", O_CREAT | O_WRONLY, 0777 );
	if( fileDescriptor == -1 )
		perror( STUDENT_FILE_OPEN_FAILED );
	else
	{
		if( no_of_students < MAX_STUDENTS )
		{
			struct student new_student;

			// Name
			write( clientSocket, ENTER_STUDENT_NAME, sizeof( ENTER_STUDENT_NAME ) );
			sleep( 1 );
			int read_name_bytes = read( clientSocket, read_buffer, 512 );
			if( read_name_bytes == -1 ) perror( "Could not read student name." );
			read_buffer[read_name_bytes - 1] = '\0';
			memset( new_student.name, 0, NAME_LENGTH );
			strcpy( new_student.name, read_buffer );
			// printf( "%s\n", new_student.name );
			memset( read_buffer, 0, 512 );

			// Age
			write( clientSocket, ENTER_STUDENT_AGE, sizeof( ENTER_STUDENT_AGE ) );
			sleep( 1 );
			int read_age_bytes = read( clientSocket, read_buffer, 512 );
			if( read_age_bytes == -1 ) perror( "Could not read student age." );
			read_buffer[read_age_bytes - 1] = '\0';
			int age                         = atoi( read_buffer );
			new_student.age                 = age;
			memset( read_buffer, 0, 512 );

			// Email
			write( clientSocket, ENTER_STUDENT_EMAIL, sizeof( ENTER_STUDENT_EMAIL ) );
			sleep( 1 );
			int read_email_bytes = read( clientSocket, read_buffer, 512 );
			if( read_email_bytes == -1 ) perror( "Could not read student email." );
			read_buffer[read_email_bytes - 1] = '\0';
			memset( new_student.email, 0, EMAIL_LENGTH );
			strcpy( new_student.email, read_buffer );
			memset( read_buffer, 0, 512 );

			// Address
			write( clientSocket, ENTER_STUDENT_ADDRESS, sizeof( ENTER_STUDENT_ADDRESS ) );
			sleep( 1 );
			int read_address_bytes = read( clientSocket, read_buffer, 512 );
			if( read_address_bytes == -1 ) perror( "Could not read student addsress." );
			read_buffer[read_address_bytes - 1] = '\0';
			memset( new_student.address, 0, ADDRESS_LENGTH );
			strcpy( new_student.address, read_buffer );
			memset( read_buffer, 0, 512 );

			// Initialize the field for no_of_offering_courses
			new_student.number_of_courses_taken = 0;

			// Memset the fields for course
			memset( new_student.enrolled_courses, 0, sizeof( new_student.enrolled_courses ) );

			// Password
			memset( new_student.password, 0, PASSWORD_LENGTH );
			strcpy( new_student.password, "student" );

			// Status
			new_student.status = 1;

			// Student ID
			// Apply Semaphores here... OR maintain the details in
			// another file...
			no_of_students++;

			// Username
			memset( new_student.username, 0, USERNAME_LENGTH );
			snprintf( new_student.username, USERNAME_LENGTH, "MT%04d", no_of_students );

			write( clientSocket, STUDENT_CREATED_SUCCESSFULLY, sizeof( STUDENT_CREATED_SUCCESSFULLY ) );

			write_student_record( fileDescriptor, &new_student, 0, sizeof( student ), 0 );
		}
		else { write( clientSocket, CANNOT_ADD_STUDENTS, sizeof( CANNOT_ADD_STUDENTS ) ); }
	}
	free( read_buffer );
	close( fileDescriptor );
}

void admin_menu_handler( int clientSocket )
{
	int condition = 1;
	while( condition )
	{
		char* read_buffer;
		read_buffer = ( char* ) malloc( 512 * sizeof( char ) );
		memset( read_buffer, 0, 512 );
		write( 1, WRITE_ADMIN_PROMPT_SUCCESSFUL, sizeof( WRITE_ADMIN_PROMPT_SUCCESSFUL ) );
		ssize_t read_bytes = read( clientSocket, read_buffer, 512 );

		if( read_bytes == -1 )
		{
			write( 2, READ_ADMIN_CHOICE_UNSUCCESSFUL, sizeof( READ_ADMIN_CHOICE_UNSUCCESSFUL ) );
		}
		else
		{
			write( 1, read_buffer, strlen( read_buffer ) );
			int userChoice = atoi( read_buffer );
			if( userChoice == 0 )
			{
				write( 1, ADMIN_NO_CHOICE_SERVER, sizeof( ADMIN_NO_CHOICE_SERVER ) );
				write( clientSocket, ADMIN_NO_CHOICE_CLIENT, sizeof( ADMIN_NO_CHOICE_CLIENT ) );
			}
			else
			{
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
				case 9:
					logout_and_exit_admin();
					condition = 0;
					break;
				default:
					write( clientSocket, ADMIN_INCORRECT_CHOICE_CLIENT,
					       sizeof( ADMIN_INCORRECT_CHOICE_CLIENT ) );
					write( 1, ADMIN_INCORRECT_CHOICE_SERVER,
					       sizeof( ADMIN_INCORRECT_CHOICE_SERVER ) );
					break;
				}
			}
		}
		sleep( 1 );
		if( condition == 1 ) write( clientSocket, ADMIN_MENU, sizeof( ADMIN_MENU ) );
		free( read_buffer );
	}
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
		int read_username_bytes = read( clientSocket, username_buffer, 16 );

		username_buffer[read_username_bytes - 1] = '\0';
		write( 1, username_buffer, strlen( username_buffer ) );
		write( clientSocket, PASSWORD_MESSAGE, sizeof( PASSWORD_MESSAGE ) );

		int read_password_bytes = read( clientSocket, password_buffer, 16 );

		write( 1, password_buffer, strlen( password_buffer ) );
		password_buffer[read_password_bytes - 1] = '\0';

		if( !strcmp( ADMIN_ID, username_buffer ) && !( strcmp( ADMIN_PASSWORD, password_buffer ) ) )
		{
			free( username_buffer );
			free( password_buffer );
			write( clientSocket, AUTHENTICATION_SUCCESS, sizeof( AUTHENTICATION_SUCCESS ) );
			sleep( 1 );
			write( clientSocket, ADMIN_MENU, sizeof( ADMIN_MENU ) );
			break;
		}
		else { write( clientSocket, AUTHENTICATION_FAILED, sizeof( AUTHENTICATION_FAILED ) ); }
	}
	admin_menu_handler( clientSocket );
}