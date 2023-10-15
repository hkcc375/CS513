#ifndef SERVER_CONSTANTS
#define SERVER_CONSTANTS

#define MAX_STUDENTS 1000
#define MAX_FACULTY 50
#define MAX_COURSES 125
#define MAX_COURSES_PER_FACULTY 5
#define MAX_COURSES_PER_STUDENT 6

#define NAME_LENGTH 32
#define DEPARTMENT_NAME_LENGTH 64
#define DESIGNATION_LENGTH 64
#define EMAIL_LENGTH 64
#define ADDRESS_LENGTH 128
#define USERNAME_LENGTH 16
#define PASSWORD_LENGTH 32
#define COURSEID_LENGTH 8

extern int no_of_students;
extern int no_of_faculty;
extern int no_of_courses;

// Role Based Prompts
#define INITIAL_PROMPT                                                         \
	".................... Welcome Back to Academia Portal :: Course "      \
	"Registration ....................\nLogin Type : Enter your choice { " \
	"1. Admin 2.Faculty 3.Student }"

#define ADMIN_MENU                                                             \
	"\n.......... Welcome to Admin Menu ..........\n1. Add Student\n2. "   \
	"View Student Details\n3. Add Faculty\n4. View Faculty Details\n5. "   \
	"Activate Student\n6. Block Student\n7. Modify Student Details\n8. "   \
	"Modify Faculty Details\n9. Logout and Exit\n\n"

#define FACULTY_MENU                                                           \
	"\n.......... Welcome to Faculty Menu ..........\n1. View Offering "   \
	"Courses\n2. Add New Course\n3. Remove Course\n4. Update Course "      \
	"Details\n5. Change Password\n6. Logout and Exit\n\n"

#define STUDENT_MENU                                                           \
	"\n.......... Welcome to Student Menu ..........\n1. View All "        \
	"Courses\n2. Enroll New Course\n3. Drop Course\n4. View Enrolled "     \
	"Course Details\n5. Change Password\n6. Logout and Exit\n\n"

// Authentication
#define LOGIN_MESSAGE "\nEnter your login id : \n"
#define PASSWORD_MESSAGE "Enter your password : \n"
#define AUTHENTICATION_FAILED                                                  \
	"Either your username or password is incorrect. \n"
#define AUTHENTICATION_SUCCESS "Login was successful. \n"
#define AUTHENTICATION_USERNAME_EMPTY "Please enter a valid username. \n"

// Student Details
#define ENTER_STUDENT_NAME "Enter Student Name : \n"
#define ENTER_STUDENT_EMAIL "Enter Student Email : \n"
#define ENTER_STUDENT_AGE "Enter Student Age : \n"
#define ENTER_STUDENT_ADDRESS "Enter Student Address : \n"
#define ENTER_STUDENT_ID "Enter Student ID : \n"

// Faculty Details
#define ENTER_FACULTY_NAME "Enter Faculty Name : \n"
#define ENTER_FACULTY_DEPARTMENT "Enter Faculty Department : \n"
#define ENTER_FACULTY_DESIGNATION "Enter Faculty Designation : \n"
#define ENTER_FACULTY_EMAIL "Enter Faculty Email : \n"
#define ENTER_FACULTY_ADDRESS "Enter Faculty Address : \n"
#define ENTER_FACULTY_ID "Enter Faculty ID : \n"

// Password Change
#define ENTER_PASSWORD "\nEnter your new password : \n"
#define ENTER_PASSWORD_CONFIRM "Please, confirm your new password : \n"
#define PASSWORDS_DO_NOT_MATCH "Passwords entered do not match. \n"
#define PASSWORDS_MATCH "\nPasswords entered match. \n"
#define PASSWORDS_CHANGED_CLIENT "Your password is now changed. \n"
#define FACULTY_PASSWORDS_CHANGED_SERVER "Faculty password is now changed. \n"
#define STUDENT_PASSWORDS_CHANGED_SERVER "Student password is now changed. \n"

// Student Record Creation/Modification, Found/Not Found
#define STUDENT_CREATED_SUCCESSFULLY "\nStudent was created successfully. \n"
#define STUDENT_RECORD_NOT_FOUND "The given student record was not found. \n"
#define STUDENT_RECORD_INVALID_CLIENT "Student id entered was invalid. \n"
#define STUDENT_RECORD_INVALID_SERVER "Admin entered invalid student id. \n"
#define STUDENT_RECORD_MODIFIED "Student record was successfully modified. \n"

// Student Active/Inactive
#define STUDENT_ACTIVE "The given student is an active student. \n"
#define STUDENT_INACTIVE "The given student is an inactive student. \n"
#define STUDENT_RECORD_BLOCKED "Student was successfully blocked. \n"
#define STUDENT_RECORD_ACTIVATED "Student was successfully activated. \n"

// Faculty Record Creation/Modification, Found/Not Found
#define FACULTY_CREATED_SUCCESSFULLY "\nFaculty was created successfully. \n"
#define FACULTY_RECORD_MODIFIED "Faculty record was successfully modified. \n"
#define FACULTY_RECORD_INVALID_SERVER "Admin entered invalid faculty id. \n"
#define FACULTY_RECORD_INVALID_CLIENT "Faculty id entered was invalid. \n"
#define FACULTY_RECORD_NOT_FOUND "The given faculty record was not found. \n"

// Faculty Operations

// Constraints on Faculty, Student and Courses
#define CANNOT_ADD_FACULTY                                                     \
	"There are 50 faculty. Cannot add any more faculty. \n"
#define CANNOT_ADD_COURSES                                                     \
	"There are 125 courses. Cannot add any more courses. \n"
#define CANNOT_ADD_STUDENTS                                                    \
	"There are 1000 students present. Cannot add any more students. \n"
#define CANNOT_ENROLL_COURSE                                                   \
	"A Student can enroll for at most 6 courses. Cannot perform Enroll "   \
	"Course. \n"
#define CANNOT_ADD_COURSE_FACULTY                                              \
	"A faculty can offer only 5 courses. Cannot perform Add Course. \n"

#endif