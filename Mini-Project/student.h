#ifndef STUDENT_H
#define STUDENT_H

struct student
{
	char name[32];
	int age;
	char email[64];
	char address[128];
	char username[16];
	int student_id;
	char password[32];
	int status; // 0 -> Inactive, 1 -> Active. By default, student is active
} student;

void student_connection_handler( int clientSocket );

#endif