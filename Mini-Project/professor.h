#ifndef FACULTY_H
#define FACULTY_H

#include "course.h"

struct Faculty
{
	char* name;
	char* department;
	char* designation;
	char* email;
	char* address;
	char* username;
	char* password;
	struct course* course_offered;
} Faculty;

#endif