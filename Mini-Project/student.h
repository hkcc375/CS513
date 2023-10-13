#ifndef STUDENT_H
#define STUDENT_H

#include "server_constants.h"

struct student
{
	char name[NAME_LENGTH];
	int age;
	char email[EMAIL_LENGTH];
	char address[ADDRESS_LENGTH];
	char username[USERNAME_LENGTH];
	int student_id;
	char password[PASSWORD_LENGTH];
	int status; // 0 -> Inactive, 1 -> Active. By default, student is active
} student;

void student_connection_handler( int clientSocket );

#endif