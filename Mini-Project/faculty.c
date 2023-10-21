#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "common.h"
#include "course_student_mapping.h"
#include "faculty.h"
#include "server_constants.h"
#include "socket_constants.h"
#include "student.h"

void update_course( int clientSocket, int faculty_id )
{

	struct flock lock_course, lock_mapping, lock_student, lock_faculty;

	int course_id;
	struct course course_record;
	struct faculty faculty_record;
	struct mapping mapping_record;
	struct student student_record;

	int fileDescriptor_course = open( "course.txt", O_CREAT | O_RDWR, 0777 );
	if( fileDescriptor_course == -1 ) perror( COURSE_FILE_OPEN_FAILED );

	int fileDescriptor_faculty = open( "faculty.txt", O_CREAT | O_RDWR, 0777 );
	if( fileDescriptor_course == -1 ) perror( FACULTY_FILE_OPEN_FAILED );

	int fileDescriptor_student = open( "student.txt", O_CREAT | O_RDWR, 0777 );
	if( fileDescriptor_student == -1 ) perror( STUDENT_FILE_OPEN_FAILED );

	int fileDescriptor_mapping = open( "mapping.txt", O_CREAT | O_RDWR, 0777 );
	if( fileDescriptor_mapping == -1 ) perror( MAPPING_FILE_OPEN_FAILED );

	lock_faculty.l_pid    = getpid();
	lock_faculty.l_whence = SEEK_SET;
	lock_faculty.l_start  = ( faculty_id - 1 ) * sizeof( struct faculty );
	lock_faculty.l_type   = F_RDLCK;
	lock_faculty.l_len    = sizeof( struct faculty );
	fcntl( fileDescriptor_faculty, F_SETLKW, &lock_faculty );

	int retval_faculty =
	          read_faculty_record( fileDescriptor_faculty, &faculty_record, faculty_id, sizeof( struct faculty ) );

	lock_faculty.l_type = F_UNLCK;
	fcntl( fileDescriptor_faculty, F_SETLKW, &lock_faculty );

	char* read_buffer = ( char* ) malloc( 512 * sizeof( char ) );
	memset( read_buffer, 0, 512 );

	write( clientSocket, ENTER_COURSE_ID, sizeof( ENTER_COURSE_ID ) );
	sleep( 1 );
	int read_course_id_bytes = read( clientSocket, read_buffer, 512 );
	if( read_course_id_bytes == -1 ) perror( "Could not read course id." );
	read_buffer[read_course_id_bytes - 1] = '\0';

	char* temp_buffer = ( char* ) malloc( 512 * sizeof( char ) );
	memset( temp_buffer, 0, 512 );
	strcpy( temp_buffer, read_buffer );
	write( 1, temp_buffer, 512 );

	char* temp_course_id_buffer = ( char* ) malloc( COURSEID_LENGTH * sizeof( char ) );
	memset( temp_course_id_buffer, 0, COURSEID_LENGTH );
	strcpy( temp_course_id_buffer, read_buffer + strlen( read_buffer ) - 3 );
	write( 1, temp_course_id_buffer, COURSEID_LENGTH );
	course_id = atoi( temp_course_id_buffer );

	if( course_id == 0 ) { write( clientSocket, ENROLL_COURSEID_EMPTY, sizeof( ENROLL_COURSEID_EMPTY ) ); }
	else
	{
		// Now we need to check if the course that the faculty entered is valid or not.
		int course_offering = 0;

		lock_course.l_pid    = getpid();
		lock_course.l_whence = SEEK_SET;
		lock_course.l_start  = ( course_id - 1 ) * sizeof( struct course );
		lock_course.l_type   = F_RDLCK;
		lock_course.l_len    = sizeof( struct course );
		fcntl( fileDescriptor_course, F_SETLKW, &lock_course );

		int retval_course = read_course_record( fileDescriptor_course, &course_record, course_id,
		                                        sizeof( struct course ) );

		lock_course.l_type = F_UNLCK;
		fcntl( fileDescriptor_course, F_SETLKW, &lock_course );

		if( retval_course == 1 )
		{
			// Course is valid. Now check if the given faculty is offering this course or not.
			int flag                    = 1;
			int check_if_course_offered = 0;
			for( int i = 0; i < MAX_COURSES_PER_FACULTY; i++ )
			{
				if( isRowEmpty( faculty_record.offering_courses, i ) )
					continue;
				else
				{
					// This means that the faculty is offering at least 1 course.
					flag = 0;
					if( !strcmp( faculty_record.offering_courses[i], read_buffer ) )
					{
						// Means that the faculty has offered the given course;
						check_if_course_offered = 1;
						break;
					}
				}
			}
			if( flag == 1 && check_if_course_offered == 0 )
			{
				write( clientSocket, NOT_OFFERING_ANY_COURSE_CLIENT,
				       sizeof( NOT_OFFERING_ANY_COURSE_CLIENT ) );
				write( 1, NOT_OFFERING_ANY_COURSE_SERVER, sizeof( NOT_OFFERING_ANY_COURSE_SERVER ) );
				sleep( 1 );
				write( clientSocket, NOT_OFFERED_THIS_COURSE_CLIENT,
				       sizeof( NOT_OFFERED_THIS_COURSE_CLIENT ) );
				write( 1, NOT_OFFERED_THIS_COURSE_SERVER, sizeof( NOT_OFFERED_THIS_COURSE_SERVER ) );
			}
			else if( flag == 0 && check_if_course_offered == 0 )
			{
				write( clientSocket, OFFERED_ANY_COURSE_CLIENT, sizeof( OFFERED_ANY_COURSE_CLIENT ) );
				write( 1, OFFERED_ANY_COURSE_SERVER, sizeof( OFFERED_ANY_COURSE_SERVER ) );
				sleep( 1 );
				write( clientSocket, NOT_OFFERED_THIS_COURSE_CLIENT,
				       sizeof( NOT_OFFERED_THIS_COURSE_CLIENT ) );
				write( 1, NOT_OFFERED_THIS_COURSE_SERVER, sizeof( NOT_OFFERED_THIS_COURSE_SERVER ) );
			}
			else
			{
				memset( read_buffer, 0, 512 );
				// This means that the faculty is offering the given course.
				write( clientSocket, ENTER_COURSE_TOTAL_SEATS, sizeof( ENTER_COURSE_TOTAL_SEATS ) );
				sleep( 1 );
				int read_total_no_of_seats_bytes = read( clientSocket, read_buffer, 512 );
				if( read_total_no_of_seats_bytes == -1 )
					perror( "Could not read total no of available seats." );
				read_buffer[read_total_no_of_seats_bytes - 1] = '\0';

				int new_total_seats = atoi( read_buffer );
				printf( "%d\n", new_total_seats );
				int filled_seats =
				          course_record.total_number_of_seats - course_record.number_of_available_seats;

				if( new_total_seats == filled_seats )
				{
					lock_course.l_type = F_WRLCK;
					fcntl( fileDescriptor_course, F_SETLKW, &lock_course );

					course_record.number_of_available_seats = 0;
					course_record.total_number_of_seats     = new_total_seats;
					// Now I have to write the course record to the course file..

					write_course_record( fileDescriptor_course, &course_record, course_id,
					                     sizeof( struct course ), 1 );

					lock_course.l_type = F_UNLCK;
					fcntl( fileDescriptor_course, F_SETLKW, &lock_course );
				}
				else if( new_total_seats > filled_seats )
				{
					// Now I have to write the course record to the course file..
					lock_course.l_type = F_WRLCK;
					fcntl( fileDescriptor_course, F_SETLKW, &lock_course );

					course_record.number_of_available_seats =
					          course_record.number_of_available_seats +
					          ( new_total_seats - course_record.total_number_of_seats );
					course_record.total_number_of_seats = new_total_seats;

					write_course_record( fileDescriptor_course, &course_record, course_id,
					                     sizeof( struct course ), 1 );

					lock_course.l_type = F_UNLCK;
					fcntl( fileDescriptor_course, F_SETLKW, &lock_course );
				}
				else if( new_total_seats < filled_seats )
				{
					// Now I have to do some shit..
					int total_no_of_students_to_remove = filled_seats - new_total_seats;
					// printf( "%d\n", total_no_of_students_to_remove );

					course_record.total_number_of_seats = new_total_seats;
					// printf( "%d\n", course_record.total_number_of_seats );

					course_record.number_of_available_seats = 0;
					// printf( "%d\n", course_record.number_of_available_seats );

					// Now I have to write the course record to the course file..
					lock_course.l_type = F_WRLCK;
					fcntl( fileDescriptor_course, F_SETLKW, &lock_course );

					write_course_record( fileDescriptor_course, &course_record, course_id,
					                     sizeof( struct course ), 1 );

					lock_course.l_type = F_UNLCK;
					fcntl( fileDescriptor_course, F_SETLKW, &lock_course );

					// Now I have to remove the course -> student mappings.
					lock_mapping.l_pid    = getpid();
					lock_mapping.l_whence = SEEK_SET;
					lock_mapping.l_start  = 0;
					lock_mapping.l_type   = F_WRLCK;
					lock_mapping.l_len    = 0;
					fcntl( fileDescriptor_mapping, F_SETLKW, &lock_mapping );

					int* student_array;
					student_array =
					          ( int* ) malloc( total_no_of_students_to_remove * sizeof( int ) );

					int num_students_removed = 0;
					off_t fileLength         = lseek( fileDescriptor_mapping, 0, SEEK_END );

					for( off_t offset = fileLength - sizeof( struct mapping );
					     offset >= 0 && num_students_removed < total_no_of_students_to_remove;
					     offset -= sizeof( struct mapping ) )
					{
						lseek( fileDescriptor_mapping, offset, SEEK_SET );
						if( read( fileDescriptor_mapping, &mapping_record,
						          sizeof( struct mapping ) ) != sizeof( struct mapping ) )
						{
							perror( "Error reading the mapping record" );
							break;
						}

						if( mapping_record.course_id == course_id )
						{

							printf( "%d\n", mapping_record.student_id );

							// Record the student ID to remove in the array
							student_array[num_students_removed] = mapping_record.student_id;
							num_students_removed++;

							// Overwrite the mapping record with zeros
							memset( &mapping_record, 0, sizeof( struct mapping ) );
							lseek( fileDescriptor_mapping, offset, SEEK_SET );
							if( write( fileDescriptor_mapping, &mapping_record,
							           sizeof( struct mapping ) ) !=
							    sizeof( struct mapping ) )
							{
								perror( "Error writing the record" );
								break;
							}
						}
					}

					// printf( "I have successfully reached here.. \n" );
					for( int i = 0; i < total_no_of_students_to_remove; i++ )
						printf( "%d\n", student_array[i] );

					lock_mapping.l_type = F_UNLCK;
					fcntl( fileDescriptor_mapping, F_SETLKW, &lock_mapping );

					// I have to remove the students that have enrolled to this course.

					for( int i = 0; i < total_no_of_students_to_remove; i++ )
					{
						int student_id = student_array[i];

						lock_student.l_pid    = getpid();
						lock_student.l_whence = SEEK_SET;
						lock_student.l_start  = student_id * sizeof( struct student );
						lock_student.l_type   = F_RDLCK;
						lock_student.l_len    = sizeof( struct student );
						fcntl( fileDescriptor_student, F_SETLKW, &lock_student );

						int retval_student =
						          read_student_record( fileDescriptor_student, &student_record,
						                               student_id, sizeof( struct student ) );
						lock_student.l_type = F_UNLCK;
						fcntl( fileDescriptor_student, F_SETLKW, &lock_student );

						for( int i = 0; i < MAX_COURSES_PER_STUDENT; i++ )
						{
							if( isRowEmpty( student_record.enrolled_courses, i ) )
								continue;
							else
							{
								// This means that the student has enrolled for at least
								// 1 course.
								flag = 0;
								if( !strcmp( student_record.enrolled_courses[i],
								             temp_buffer ) )
								{

									lock_student.l_type = F_WRLCK;
									fcntl( fileDescriptor_student, F_SETLKW,
									       &lock_student );

									// Memset the field for this course.
									memset( &student_record.enrolled_courses[i], 0,
									        COURSEID_LENGTH );
									// I have to decrease the number of courses
									// taken by the student
									student_record.number_of_courses_taken -= 1;
									write_student_record(
									          fileDescriptor_student,
									          &student_record, student_id,
									          sizeof( struct student ), 1 );

									lock_student.l_type = F_UNLCK;
									fcntl( fileDescriptor_student, F_SETLKW,
									       &lock_student );

									break;
								}
							}
						}
					}
					free( student_array );
				}
				// Now write that the course was successfully updated.
				write( clientSocket, COURSE_SUCCESSFULLY_UPDATED,
				       sizeof( COURSE_SUCCESSFULLY_UPDATED ) );
				write( 1, COURSE_SUCCESSFULLY_UPDATED, sizeof( COURSE_SUCCESSFULLY_UPDATED ) );
			}
		}
		else
		{
			// Course is not found. Invalid course.
			write( clientSocket, COURSE_RECORD_NOT_FOUND, sizeof( COURSE_RECORD_NOT_FOUND ) );
		}
	}
	close( fileDescriptor_course );
	close( fileDescriptor_mapping );
	close( fileDescriptor_faculty );
	close( fileDescriptor_student );
	free( read_buffer );
	free( temp_buffer );
	free( temp_course_id_buffer );
}

void remove_course_from_catalog( int clientSocket, int faculty_id )
{
	struct flock lock_course, lock_faculty, lock_student, lock_mapping;

	struct faculty faculty_record;
	struct course course_record;
	struct mapping mapping_record;
	struct student student_record;
	int course_id;
	int fileDescriptor_faculty = open( "faculty.txt", O_CREAT | O_RDWR, 0777 );
	if( fileDescriptor_faculty == -1 ) perror( FACULTY_FILE_OPEN_FAILED );

	int fileDescriptor_student = open( "student.txt", O_CREAT | O_RDWR, 0777 );
	if( fileDescriptor_student == -1 ) perror( STUDENT_FILE_OPEN_FAILED );

	int fileDescriptor_course = open( "course.txt", O_CREAT | O_RDWR, 0777 );
	if( fileDescriptor_course == -1 ) perror( COURSE_FILE_OPEN_FAILED );

	int fileDescriptor_mapping = open( "mapping.txt", O_CREAT | O_RDWR, 0777 );
	if( fileDescriptor_mapping == -1 ) perror( MAPPING_FILE_OPEN_FAILED );

	lock_faculty.l_pid    = getpid();
	lock_faculty.l_whence = SEEK_SET;
	lock_faculty.l_start  = ( faculty_id - 1 ) * sizeof( struct faculty );
	lock_faculty.l_type   = F_RDLCK;
	lock_faculty.l_len    = sizeof( struct faculty );
	fcntl( fileDescriptor_faculty, F_SETLKW, &lock_faculty );

	int retval_faculty =
	          read_faculty_record( fileDescriptor_faculty, &faculty_record, faculty_id, sizeof( struct faculty ) );

	lock_faculty.l_type = F_UNLCK;
	fcntl( fileDescriptor_faculty, F_SETLKW, &lock_faculty );

	char* read_buffer = ( char* ) malloc( 512 * sizeof( char ) );
	memset( read_buffer, 0, 512 );

	write( clientSocket, ENTER_COURSE_ID, sizeof( ENTER_COURSE_ID ) );
	sleep( 1 );
	int read_course_id_bytes = read( clientSocket, read_buffer, 512 );
	if( read_course_id_bytes == -1 ) perror( "Could not read course id." );
	read_buffer[read_course_id_bytes - 1] = '\0';

	char* temp_course_id_buffer = ( char* ) malloc( COURSEID_LENGTH * sizeof( char ) );
	memset( temp_course_id_buffer, 0, COURSEID_LENGTH );
	strcpy( temp_course_id_buffer, read_buffer + strlen( read_buffer ) - 3 );
	write( 1, temp_course_id_buffer, COURSEID_LENGTH );
	course_id = atoi( temp_course_id_buffer );

	if( course_id == 0 ) { write( clientSocket, ENROLL_COURSEID_EMPTY, sizeof( ENROLL_COURSEID_EMPTY ) ); }
	else
	{
		int course_removed = 0;
		sleep( 1 );

		lock_course.l_pid    = getpid();
		lock_course.l_whence = SEEK_SET;
		lock_course.l_start  = ( course_id - 1 ) * sizeof( struct course );
		lock_course.l_type   = F_RDLCK;
		lock_course.l_len    = sizeof( struct course );
		fcntl( fileDescriptor_course, F_SETLKW, &lock_course );

		int retval_course = read_course_record( fileDescriptor_course, &course_record, course_id,
		                                        sizeof( struct course ) );

		lock_course.l_type = F_UNLCK;
		fcntl( fileDescriptor_course, F_SETLKW, &lock_course );

		if( retval_course == 1 )
		{
			int flag                    = 1;
			int check_if_course_offered = 0;
			for( int i = 0; i < MAX_COURSES_PER_FACULTY; i++ )
			{
				if( isRowEmpty( faculty_record.offering_courses, i ) )
					continue;
				else
				{
					// This means that the faculty is offering at least 1 course.
					flag = 0;
					if( !strcmp( faculty_record.offering_courses[i], read_buffer ) )
					{
						// Means that the faculty has offered the given course;
						check_if_course_offered = 1;

						lock_faculty.l_type = F_WRLCK;
						fcntl( fileDescriptor_faculty, F_SETLKW, &lock_faculty );

						// Memset the field for this course.
						memset( &faculty_record.offering_courses[i], 0, COURSEID_LENGTH );

						// I have to decrease the number of courses taken by the faculty
						faculty_record.no_of_offering_courses -= 1;

						// We reduce the number of courses
						// no_of_courses--;

						write_faculty_record( fileDescriptor_faculty, &faculty_record,
						                      faculty_id, sizeof( struct faculty ), 1 );

						lock_faculty.l_type = F_UNLCK;
						fcntl( fileDescriptor_faculty, F_SETLKW, &lock_faculty );

						break;
					}
				}
			}
			if( flag == 1 && check_if_course_offered == 0 )
			{
				write( clientSocket, NOT_OFFERING_ANY_COURSE_CLIENT,
				       sizeof( NOT_OFFERING_ANY_COURSE_CLIENT ) );
				write( 1, NOT_OFFERING_ANY_COURSE_SERVER, sizeof( NOT_OFFERING_ANY_COURSE_SERVER ) );
				sleep( 1 );
				write( clientSocket, NOT_OFFERED_THIS_COURSE_CLIENT,
				       sizeof( NOT_OFFERED_THIS_COURSE_CLIENT ) );
				write( 1, NOT_OFFERED_THIS_COURSE_SERVER, sizeof( NOT_OFFERED_THIS_COURSE_SERVER ) );
			}
			else if( flag == 0 && check_if_course_offered == 0 )
			{
				write( clientSocket, OFFERED_ANY_COURSE_CLIENT, sizeof( OFFERED_ANY_COURSE_CLIENT ) );
				write( 1, OFFERED_ANY_COURSE_SERVER, sizeof( OFFERED_ANY_COURSE_SERVER ) );
				sleep( 1 );
				write( clientSocket, NOT_OFFERED_THIS_COURSE_CLIENT,
				       sizeof( NOT_OFFERED_THIS_COURSE_CLIENT ) );
				write( 1, NOT_OFFERED_THIS_COURSE_SERVER, sizeof( NOT_OFFERED_THIS_COURSE_SERVER ) );
			}
			else
			{
				lock_student.l_pid    = getpid();
				lock_student.l_whence = SEEK_SET;
				lock_student.l_start  = 0;
				lock_student.l_type   = F_WRLCK;
				lock_student.l_len    = 0;
				fcntl( fileDescriptor_student, F_SETLKW, &lock_student );

				// Remove the course from the student side
				int bytesToRead_student;
				int student_no = 1;
				while( ( bytesToRead_student = read( fileDescriptor_student, &student_record,
				                                     sizeof( struct student ) ) ) > 0 )
				{
					// We have reached the EOF
					if( bytesToRead_student != sizeof( struct student ) )
					{
						perror( STUDENT_RECORD_EOF );
					}

					for( int i = 0; i < MAX_COURSES_PER_STUDENT; i++ )
					{
						if( isRowEmpty( student_record.enrolled_courses, i ) )
							continue;
						else
						{
							if( !strcmp( student_record.enrolled_courses[i], read_buffer ) )
							{
								// Memset the field for this course.
								memset( &student_record.enrolled_courses[i], 0,
								        COURSEID_LENGTH );
								// I have to decrease the number of courses taken by the
								// student
								student_record.number_of_courses_taken -= 1;
								write_student_record( fileDescriptor_student,
								                      &student_record, student_no,
								                      sizeof( struct student ), 1 );
								break;
							}
						}
					}
					student_no++;
				}

				lock_student.l_type = F_UNLCK;
				fcntl( fileDescriptor_student, F_SETLKW, &lock_student );

				lock_mapping.l_pid    = getpid();
				lock_mapping.l_whence = SEEK_SET;
				lock_mapping.l_start  = 0;
				lock_mapping.l_type   = F_WRLCK;
				lock_mapping.l_len    = 0;
				fcntl( fileDescriptor_mapping, F_SETLKW, &lock_mapping );

				// Remove the Mappings
				int bytesToRead_mapping;
				int mapping_no = 0;
				while( ( bytesToRead_mapping = read( fileDescriptor_mapping, &mapping_record,
				                                     sizeof( struct mapping ) ) ) > 0 )
				{
					// We have reached the EOF
					if( bytesToRead_mapping != sizeof( struct course ) )
					{
						perror( MAPPING_RECORD_EOF );
					}

					if( mapping_record.course_id == course_id )
					{
						// Memset the entire structure.

						memset( &mapping_record, 0, sizeof( struct mapping ) );
						write_mapping_record( fileDescriptor_mapping, &mapping_record,
						                      mapping_no + 1, sizeof( struct mapping ), 1 );
					}
					mapping_no++;
				}

				lock_mapping.l_type = F_UNLCK;
				fcntl( fileDescriptor_mapping, F_SETLKW, &lock_mapping );

				lock_course.l_pid    = getpid();
				lock_course.l_whence = SEEK_SET;
				lock_course.l_start  = ( course_id - 1 ) * sizeof( struct course );
				lock_course.l_type   = F_WRLCK;
				lock_course.l_len    = sizeof( struct course );
				fcntl( fileDescriptor_course, F_SETLKW, &lock_course );

				// We need to memset the fields in the course record;
				memset( &course_record, 0, sizeof( struct course ) );
				write_course_record( fileDescriptor_course, &course_record, course_id,
				                     sizeof( struct course ), 1 );

				lock_course.l_type = F_UNLCK;
				fcntl( fileDescriptor_course, F_SETLKW, &lock_course );

				sleep( 1 );
				write( clientSocket, SUCCESSFULLY_REMOVED_COURSE,
				       sizeof( SUCCESSFULLY_REMOVED_COURSE ) );
				write( 1, SUCCESSFULLY_REMOVED_COURSE, sizeof( SUCCESSFULLY_REMOVED_COURSE ) );
			}
		}
		else
		{
			// The course was not found...
			write( clientSocket, COURSE_RECORD_NOT_FOUND, sizeof( COURSE_RECORD_NOT_FOUND ) );
		}
	}
	free( read_buffer );
	close( fileDescriptor_course );
	close( fileDescriptor_faculty );
	close( fileDescriptor_mapping );
	close( fileDescriptor_student );
}

void view_offering_courses( int clientSocket, int faculty_id )
{
	struct flock lock_course, lock_faculty;

	struct faculty faculty_record;
	struct course course_record;

	char* read_buffer = ( char* ) malloc( 512 * sizeof( char ) );
	memset( read_buffer, 0, 512 );

	int fileDescriptor_faculty = open( "faculty.txt", O_CREAT | O_RDONLY, 0777 );
	if( fileDescriptor_faculty == -1 ) perror( FACULTY_FILE_OPEN_FAILED );
	int fileDescriptor_course = open( "course.txt", O_CREAT | O_RDONLY, 0777 );
	if( fileDescriptor_course == -1 ) perror( COURSE_FILE_OPEN_FAILED );

	lock_faculty.l_pid    = getpid();
	lock_faculty.l_whence = SEEK_SET;
	lock_faculty.l_start  = ( faculty_id - 1 ) * sizeof( struct faculty );
	lock_faculty.l_type   = F_RDLCK;
	lock_faculty.l_len    = sizeof( struct faculty );
	fcntl( fileDescriptor_faculty, F_SETLKW, &lock_faculty );

	int retval_student =
	          read_faculty_record( fileDescriptor_faculty, &faculty_record, faculty_id, sizeof( struct faculty ) );

	lock_faculty.l_type = F_UNLCK;
	fcntl( fileDescriptor_faculty, F_SETLKW, &lock_faculty );

	int flag = 1;
	for( int i = 0; i < MAX_COURSES_PER_FACULTY; i++ )
	{
		if( isRowEmpty( faculty_record.offering_courses, i ) )
			continue;
		else
		{
			// This means that the faculty is offering at least 1 course.
			flag              = 0;
			char* temp_buffer = ( char* ) malloc( COURSEID_LENGTH * sizeof( char ) );
			memset( temp_buffer, 0, COURSEID_LENGTH );
			strcpy( temp_buffer,
			        faculty_record.offering_courses[i] + strlen( faculty_record.offering_courses[i] ) - 3 );
			int course_id = atoi( temp_buffer );
			free( temp_buffer );
			// We now have to show the course details...

			lock_course.l_pid    = getpid();
			lock_course.l_whence = SEEK_SET;
			lock_course.l_start  = ( course_id - 1 ) * sizeof( struct course );
			lock_course.l_type   = F_RDLCK;
			lock_course.l_len    = sizeof( struct course );
			fcntl( fileDescriptor_course, F_SETLKW, &lock_course );

			int retval_course = read_course_record( fileDescriptor_course, &course_record, course_id,
			                                        sizeof( struct course ) );

			lock_course.l_type = F_UNLCK;
			fcntl( fileDescriptor_course, F_SETLKW, &lock_course );

			if( retval_course == 1 )
			{
				// Show course details here..
				// Course Name
				strcat( read_buffer, "Course Name : " );
				strcat( read_buffer, course_record.course_name );
				strcat( read_buffer, "\n" );
				write( clientSocket, read_buffer, strlen( read_buffer ) );
				memset( read_buffer, 0, 512 );

				// Course ID
				strcat( read_buffer, "Course ID : " );
				strcat( read_buffer, course_record.course_id );
				strcat( read_buffer, "\n" );
				write( clientSocket, read_buffer, strlen( read_buffer ) );
				memset( read_buffer, 0, 512 );

				// Course Department
				strcat( read_buffer, "Department : " );
				strcat( read_buffer, course_record.department );
				strcat( read_buffer, "\n" );
				write( clientSocket, read_buffer, strlen( read_buffer ) );
				memset( read_buffer, 0, 512 );

				// Course Credits
				strcat( read_buffer, "Credits : " );
				char* temp_buffer_credits = ( char* ) malloc( 10 * sizeof( char ) );
				snprintf( temp_buffer_credits, 10, "%d", course_record.credits );
				strcat( read_buffer, temp_buffer_credits );
				strcat( read_buffer, "\n" );
				write( clientSocket, read_buffer, strlen( read_buffer ) );
				free( temp_buffer_credits );
				memset( read_buffer, 0, 512 );

				// Course Total No. Of Seats
				strcat( read_buffer, "Total No. Of Seats : " );
				char* temp_buffer_total_seats = ( char* ) malloc( 10 * sizeof( char ) );
				snprintf( temp_buffer_total_seats, 10, "%d", course_record.total_number_of_seats );
				strcat( read_buffer, temp_buffer_total_seats );
				strcat( read_buffer, "\n" );
				write( clientSocket, read_buffer, strlen( read_buffer ) );
				free( temp_buffer_total_seats );
				memset( read_buffer, 0, 512 );

				// Course Total No. Of Available Seats
				strcat( read_buffer, "Total No. Of Available Seats : " );
				char* temp_buffer_total_available_seats = ( char* ) malloc( 10 * sizeof( char ) );
				snprintf( temp_buffer_total_available_seats, 10, "%d",
				          course_record.number_of_available_seats );
				strcat( read_buffer, temp_buffer_total_available_seats );
				strcat( read_buffer, "\n" );
				strcat( read_buffer, "\n" );
				write( clientSocket, read_buffer, strlen( read_buffer ) );
				free( temp_buffer_total_available_seats );
				memset( read_buffer, 0, 512 );
			}
		}
	}
	if( flag == 1 )
	{
		write( clientSocket, NOT_OFFERING_ANY_COURSE_CLIENT, sizeof( NOT_OFFERING_ANY_COURSE_CLIENT ) );
		write( 1, NOT_OFFERING_ANY_COURSE_SERVER, sizeof( NOT_OFFERING_ANY_COURSE_SERVER ) );
	}
	free( read_buffer );
	close( fileDescriptor_course );
	close( fileDescriptor_faculty );
}

void add_new_course( int clientSocket, int faculty_id )
{
	struct flock lock_course, lock_faculty;

	char* read_buffer;
	read_buffer = ( char* ) malloc( 512 * sizeof( char ) );
	memset( read_buffer, 0, 512 );

	int fileDescriptor_course = open( "course.txt", O_CREAT | O_RDWR, 0777 );
	if( fileDescriptor_course == -1 )
		perror( COURSE_FILE_OPEN_FAILED );
	else
	{
		lock_course.l_pid    = getpid();
		lock_course.l_whence = SEEK_SET;
		lock_course.l_start  = 0;
		lock_course.l_type   = F_RDLCK;
		lock_course.l_len    = 0;
		fcntl( fileDescriptor_course, F_SETLKW, &lock_course );

		int bytesToRead;
		int no_of_courses_in_file = 0;
		struct course old_record;
		while( ( bytesToRead = read( fileDescriptor_course, &old_record, sizeof( struct course ) ) > 0 ) )
		{
			// We have reached the EOF
			if( bytesToRead != sizeof( struct course ) ) { perror( MAPPING_RECORD_EOF ); }

			if( isStructEmpty( &old_record ) ) { continue; }
			else { no_of_courses_in_file++; }
		}

		lock_course.l_type = F_UNLCK;
		fcntl( fileDescriptor_course, F_SETLKW, &lock_course );

		int fileDescriptor_faculty = open( "faculty.txt", O_CREAT | O_RDWR, 0777 );
		if( fileDescriptor_faculty == -1 )
			perror( FACULTY_FILE_OPEN_FAILED );
		else
		{

			lock_faculty.l_pid    = getpid();
			lock_faculty.l_whence = SEEK_SET;
			lock_faculty.l_start  = ( faculty_id - 1 ) * sizeof( struct faculty );
			lock_faculty.l_type   = F_RDLCK;
			lock_faculty.l_len    = sizeof( struct faculty );
			fcntl( fileDescriptor_faculty, F_SETLKW, &lock_faculty );

			struct faculty f_record;
			int retval = read_faculty_record( fileDescriptor_faculty, &f_record, faculty_id,
			                                  sizeof( struct faculty ) );

			lock_faculty.l_type = F_UNLCK;
			fcntl( fileDescriptor_faculty, F_SETLKW, &lock_faculty );

			if( retval == 1 )
			{
				if( f_record.no_of_offering_courses < MAX_COURSES_PER_FACULTY )
				{
					if( no_of_courses_in_file < MAX_COURSES )
					{
						struct course new_course;
						memset( read_buffer, 0, 512 );

						int index;
						// Find the first point to insert the course.
						for( int i = 0; i < MAX_COURSES_PER_STUDENT; i++ )
						{
							if( isRowEmpty( f_record.offering_courses, i ) )
							{
								index = i;
								break;
							}
						}

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

						lock_faculty.l_pid    = getpid();
						lock_faculty.l_whence = SEEK_SET;
						lock_faculty.l_start  = ( faculty_id - 1 ) * sizeof( struct faculty );
						lock_faculty.l_type   = F_WRLCK;
						lock_faculty.l_len    = sizeof( struct faculty );
						fcntl( fileDescriptor_faculty, F_SETLKW, &lock_faculty );

						lock_course.l_pid    = getpid();
						lock_course.l_whence = SEEK_SET;
						lock_course.l_start  = 0;
						lock_course.l_type   = F_WRLCK;
						lock_course.l_len    = 0;
						fcntl( fileDescriptor_course, F_SETLKW, &lock_course );

						no_of_courses++;
						// Course ID
						memset( new_course.course_id, 0, COURSEID_LENGTH );
						snprintf( new_course.course_id, COURSEID_LENGTH, "CC%03d",
						          no_of_courses );
						strcpy( &f_record.offering_courses[index - 1][COURSEID_LENGTH],
						        new_course.course_id );

						// Incrementing the number of
						// courses that a faculty wants
						// to add
						f_record.no_of_offering_courses++;

						write_faculty_record( fileDescriptor_faculty, &f_record, faculty_id,
						                      sizeof( struct faculty ), 1 );

						lock_faculty.l_type = F_UNLCK;
						fcntl( fileDescriptor_faculty, F_SETLKW, &lock_faculty );

						write( clientSocket, FACULTY_RECORD_MODIFIED,
						       sizeof( FACULTY_RECORD_MODIFIED ) );
						write( 1, FACULTY_RECORD_MODIFIED, sizeof( FACULTY_RECORD_MODIFIED ) );
						sleep( 1 );
						write( clientSocket, COURSE_CREATED_SUCCESSFULLY,
						       sizeof( COURSE_CREATED_SUCCESSFULLY ) );
						write( 1, COURSE_CREATED_SUCCESSFULLY,
						       sizeof( COURSE_CREATED_SUCCESSFULLY ) );

						// Any changes, make them here...

						write_course_record( fileDescriptor_course, &new_course, 0,
						                     sizeof( struct course ), 0 );

						lock_course.l_type = F_UNLCK;
						fcntl( fileDescriptor_course, F_SETLKW, &lock_course );
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
	struct flock lock;

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

			lock.l_pid    = getpid();
			lock.l_whence = SEEK_SET;
			lock.l_start  = ( faculty_id - 1 ) * sizeof( struct faculty );
			lock.l_type   = F_RDLCK;
			lock.l_len    = sizeof( struct faculty );
			fcntl( fileDescriptor, F_SETLKW, &lock );

			int retval =
			          read_faculty_record( fileDescriptor, &record, faculty_id, sizeof( struct faculty ) );

			lock.l_type = F_UNLCK;
			fcntl( fileDescriptor, F_SETLKW, &lock );

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

				lock.l_type = F_WRLCK;
				fcntl( fileDescriptor, F_SETLKW, &lock );

				write_faculty_record( fileDescriptor, &record, faculty_id, sizeof( faculty ), 1 );

				lock.l_type = F_UNLCK;
				fcntl( fileDescriptor, F_SETLKW, &lock );

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
				case 1:
					loadVariablesFromFile();
					add_new_course( clientSocket, faculty_id );
					saveVariablesToFile();
					break;
				case 2:
					loadVariablesFromFile();
					remove_course_from_catalog( clientSocket, faculty_id );
					saveVariablesToFile();
					break;
				case 3:
					loadVariablesFromFile();
					view_offering_courses( clientSocket, faculty_id );
					saveVariablesToFile();
					break;
				case 4:
					loadVariablesFromFile();
					update_course( clientSocket, faculty_id );
					break;
					saveVariablesToFile();
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
	struct flock lock;

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
				lock.l_pid    = getpid();
				lock.l_whence = SEEK_SET;
				lock.l_start  = ( faculty_id - 1 ) * sizeof( struct faculty );
				lock.l_type   = F_RDLCK;
				lock.l_len    = sizeof( struct faculty );
				fcntl( fileDescriptor, F_SETLKW, &lock );

				int retval = read_faculty_record( fileDescriptor, &record, faculty_id,
				                                  sizeof( struct faculty ) );

				lock.l_type = F_UNLCK;
				fcntl( fileDescriptor, F_SETLKW, &lock );

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