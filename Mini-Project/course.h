#ifndef COURSE_H
#define COURSE_H

#include "server_constants.h"

struct course
{
	char course_id[COURSEID_LENGTH];
	char course_name[NAME_LENGTH];
	char department[DEPARTMENT_NAME_LENGTH];
	int credits;
	int total_number_of_seats;
	int number_of_available_seats;
	// int status;
} course;

#endif