#ifndef COURSE_H
#define COURSE_H

struct course
{
	char* course_id;
	char* department;
	int credits;
	int total_number_of_seats;
	int number_of_available_seats;
} course;

#endif