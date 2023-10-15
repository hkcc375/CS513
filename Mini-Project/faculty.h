#ifndef FACULTY_H
#define FACULTY_H

#include "course.h"
#include "server_constants.h"

struct faculty
{
	char name[NAME_LENGTH];
	char department[DEPARTMENT_NAME_LENGTH];
	char designation[DESIGNATION_LENGTH];
	char email[EMAIL_LENGTH];
	char address[ADDRESS_LENGTH];
	char username[USERNAME_LENGTH];
	char password[PASSWORD_LENGTH];
	int no_of_offering_courses;
	char offering_courses[MAX_COURSES_PER_FACULTY][COURSEID_LENGTH];
} faculty;

void faculty_connection_handler( int clientSocket );
void faculty_menu_handler( int clientSocket, int faculty_id );
void add_new_course( int clientSocket, int faculty_id );
void view_offering_courses( int clientSocket, int faculty_id );
void remove_course_from_catalog( int clientSocket, int faculty_id );
void update_course( int clientSocket, int faculty_id );
void change_password_faculty( int clientSocket, int faculty_id );
void logout_and_exit_faculty();

#endif