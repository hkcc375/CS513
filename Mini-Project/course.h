#ifndef COURSE_H
#define COURSE_H

struct course
{
	char courseid[8];
	char department[32];
	int credits;
	int total_number_of_seats;
	int number_of_available_seats;
} course;

#endif