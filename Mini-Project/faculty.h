#ifndef FACULTY_H
#define FACULTY_H

#include "course.h"

struct faculty
{
	char* name;
	char* department;
	char* designation;
	char* email;
	char* address;
	char* username;
	char* password;
	struct course* course_offered;
} faculty;

void faculty_connection_handler( int clientSocket );

#endif