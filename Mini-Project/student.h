#ifndef STUDENT_H
#define STUDENT_H

struct student
{
	char* name;
	int age;
	char* email;
	char* address;
	char* username;
	char* password;
	int status;
} student;

void student_handler( int clientSocket );

#endif