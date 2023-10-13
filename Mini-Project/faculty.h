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
	int faculty_id;
	char password[PASSWORD_LENGTH];
	// struct course* course_offered;
} faculty;

void faculty_connection_handler( int clientSocket );

#endif