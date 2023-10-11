#ifndef ADMIN_H
#define ADMIN_H

#define ADMIN_ID "admin"
#define ADMIN_PASSWORD "1234"

void admin_connection_handler( int clientSocket );
void admin_menu_handler( int clientSocket );
void add_student( int clientSocket );
void add_faculty( int clientSocket );
void view_student_details( int clientSocket );
void view_faculty_details( int clientSocket );
void activate_student( int clientSocket );
void block_student( int clientSocket );
void modify_student_details( int clientSocket );
void modify_faculty_details( int clientSocket );
void logout_and_exit( int clientSocket );

#endif