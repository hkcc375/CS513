#ifndef STUDENT_H
#define STUDENT_H

#include "faculty.h"
#include "server_constants.h"

struct student
{
	char name[NAME_LENGTH];
	int age;
	char email[EMAIL_LENGTH];
	char address[ADDRESS_LENGTH];
	char username[USERNAME_LENGTH];
	char password[PASSWORD_LENGTH];
	int status; // 0 -> Inactive, 1 -> Active. By default, student is active
} student;

void student_connection_handler( int clientSocket );
void student_menu_handler( int clientSocket, int student_id );
void enroll_course( int clientSocket, int student_id );
void drop_course( int clientSocket, int student_id );
void view_all_courses( int clientSocket, int student_id );
void view_enrolled_courses( int clientSocket, int student_id );
void change_password_student( int clientSocket, int student_id );
void logout_and_exit_student();

#endif