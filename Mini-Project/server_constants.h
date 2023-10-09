#ifndef SERVER_CONSTANTS
#define SERVER_CONSTANTS

#define MAX_STUDENTS 500
#define MAX_FACULTY 50
#define MAX_COURSES 100

#define STUDENT_NAME_LENGTH 32
#define DEPARTMENT_NAME_LENGTH 32
#define DESIGNATION_LENGTH 32
#define EMAIL_LENGTH 64
#define ADDRESS_LENGTH 128
#define USERNAME_LENGTH 16
#define PASSWORD_LENGTH 32
#define COURSEID_LENGTH 8

extern int no_of_students;
extern int no_of_faculty;
extern int no_of_courses;

#define INITIAL_PROMPT                                                         \
	".................... Welcome Back to Academia Portal :: Course "      \
	"Registration ....................\nLogin Type : Enter your choice { " \
	"1. Admin 2.Faculty 3.Student }"

#define ADMIN_MENU                                                             \
	".......... Welcome to Admin Menu ..........\n1. Add Student\n2. "     \
	"View Student Details\n3. Add Faculty\n4. View Faculty Details\n5. "   \
	"Activate Student\n6. Block Student\n7. Modify Student Details\n8. "   \
	"Modify Faculty Details\n9. Logout and Exit"

#define FACULTY_MENU                                                           \
	".......... Welcome to Faculty Menu ..........\n1. View Offering "     \
	"Courses\n2. Add New Course\n3. Remove Course\n4. Update Course "      \
	"Details\n5. Change Password\n6. Logout and Exit"

#define STUDENT_MENU                                                           \
	".......... Welcome to Student Menu ..........\n1. View All "          \
	"Courses\n2. Enroll New Course\n3. Drop Course\n4. View Enrolled "     \
	"Course Details\n5. Change Password\n6. Logout and Exit"

#endif