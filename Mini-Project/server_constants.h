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
#define INITIAL_PROMPT                                                                                                 \
	".................... Welcome Back to Academia Portal :: Course "                                              \
	"Registration ....................\nLogin Type : Enter your choice { "                                         \
	"1. Admin 2.Faculty 3.Student }"

#define ADMIN_MENU                                                                                                     \
	"\n.......... Welcome to Admin Menu ..........\n1. Add Student\n2. "                                           \
	"View Student Details\n3. Add Faculty\n4. View Faculty Details\n5. "                                           \
	"Activate Student\n6. Block Student\n7. Modify Student Details\n8. "                                           \
	"Modify Faculty Details\n9. Logout and Exit\n\n"

#define FACULTY_MENU                                                                                                   \
	"\n.......... Welcome to Faculty Menu ..........\n1. Add New "                                                 \
	"Course\n2. Remove Course\n3. View Offering Courses\n4. Update "                                               \
	"Course "                                                                                                      \
	"Details\n5. Change Password\n6. Logout and Exit\n\n"

#define STUDENT_MENU                                                                                                   \
	"\n.......... Welcome to Student Menu ..........\n1. Enroll New "                                              \
	"Course\n2. Drop Course\n3. View All Courses\n4. View Enrolled "                                               \
	"Course Details\n5. Change Password\n6. Logout and Exit\n\n"

// Authentication
#define LOGIN_MESSAGE "\nEnter your login id : \n"
#define PASSWORD_MESSAGE "Enter your password : \n"
#define AUTHENTICATION_FAILED "Either your username or password is incorrect. \n"
#define AUTHENTICATION_SUCCESS "Login was successful. \n"
#define AUTHENTICATION_USERNAME_EMPTY "Please enter a valid username. \n"

#define ENROLL_COURSEID_EMPTY "Please enter a valid course id. \n"

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

// Course Details
#define ENTER_COURSE_ID "Enter Course ID : \n"
#define ENTER_COURSE_NAME "Enter Course Name : \n"
#define ENTER_COURSE_DEPARTMENT "Enter Course Department : \n"
#define ENTER_COURSE_CREDITS "Enter Course Credits : \n"
#define ENTER_COURSE_TOTAL_SEATS "Enter Course Total No. Of Seats : \n"
#define ENTER_COURSE_TOTAL_AVAILABLE_SEATS "Enter Course Total No. Of Available Seats : \n"

// Password Change
#define ENTER_PASSWORD "\nEnter your new password : \n"
#define ENTER_PASSWORD_CONFIRM "Please, confirm your new password : \n"
#define PASSWORDS_DO_NOT_MATCH "Passwords entered do not match. \n"
#define PASSWORDS_MATCH_SERVER "Passwords entered match. \n"
#define PASSWORDS_MATCH_CLIENT "\nPasswords entered match. \n"
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
#define STUDENT_INACTIVE_LOGIN "You are inactive student. \n"
#define STUDENT_INACTIVE "The given student is an inactive student. \n"
#define STUDENT_RECORD_BLOCKED "Student was successfully blocked. \n"
#define STUDENT_RECORD_ACTIVATED "Student was successfully activated. \n"

// Faculty Record Creation/Modification, Found/Not Found
#define FACULTY_CREATED_SUCCESSFULLY "\nFaculty was created successfully. \n"
#define FACULTY_RECORD_MODIFIED "Faculty record was successfully modified. \n"
#define FACULTY_RECORD_INVALID_SERVER "Admin entered invalid faculty id. \n"
#define FACULTY_RECORD_INVALID_CLIENT "Faculty id entered was invalid. \n"
#define FACULTY_RECORD_NOT_FOUND "The given faculty record was not found. \n"

// Course Record Creation/Modification, Found/Not Found
#define COURSE_RECORD_NOT_FOUND "The given course record was not found. \n"
#define COURSE_CREATED_SUCCESSFULLY "\nCourse was created successfully. \n"
#define COURSE_RECORD_MODIFIED "Course record was successfully modified. \n"

// Course Active/Inactive
#define COURSE_ACTIVE "The given student is an active student. \n"
#define COURSE_INACTIVE "The given student is an inactive student. \n"
#define COURSE_RECORD_BLOCKED "Student was successfully blocked. \n"
#define COURSE_RECORD_ACTIVATED "Student was successfully activated. \n"

// Constraints on Faculty, Student and Courses
#define CANNOT_ADD_FACULTY "There are 50 faculty. Cannot add any more faculty. \n"
#define CANNOT_ADD_COURSES "Faculty can add a course, but there are 125 courses already. \n"
#define CANNOT_ADD_STUDENTS "There are 1000 students present. Cannot add any more students. \n"
#define CANNOT_ENROLL_COURSE                                                                                           \
	"A Student can enroll for at most 6 courses. Cannot perform Enroll "                                           \
	"Course. \n"
#define CANNOT_ENROLL_COURSE_MAX_LIMIT_REACHED_STUDENT                                                                 \
	"This course has all the seats filled. You cannot enroll to this "                                             \
	"course. \n"
#define CANNOT_ADD_COURSE_FACULTY "A faculty can offer only 5 courses. Cannot perform Add Course. \n"
#define COURSE_ALREADY_ENROLLED_CLIENT                                                                                 \
	"You have already enrolled for this course. Cannot perform Enroll "                                            \
	"Course. \n"
#define COURSE_ALREADY_ENROLLED_SERVER                                                                                 \
	"Student has already enrolled for this course. Cannot perform Enroll "                                         \
	"Course. \n"

#define ENROLL_COURSE_SUCCESSFUL_CLIENT "You have successfully enrolled to this course. \n"
#define ENROLL_COURSE_SUCCESSFUL_SERVER "Student has successfully enrolled to this course. \n"

#define ENROLLED_ANY_COURSE_SERVER "Student has enrolled for at least 1 course. \n"
#define ENROLLED_ANY_COURSE_CLIENT "You have enrolled for at least 1 course. \n"
#define NOT_ENROLLED_ANY_COURSE_CLIENT "You have not enrolled for any courses. \n"
#define NOT_ENROLLED_THIS_COURSE_CLIENT "You have not enrolled for this course. Cannot perform Drop Course. \n"
#define NOT_ENROLLED_ANY_COURSE_SERVER "The student has not enrolled for any courses. \n"
#define NOT_ENROLLED_THIS_COURSE_SERVER "The student has not enrolled for this course. Cannot perform Drop Course. \n"

#define NOT_OFFERING_ANY_COURSE_SERVER "The faculty is not offering any course. \n"
#define NOT_OFFERING_ANY_COURSE_CLIENT "You are not offering any courses right now. \n"
#define NOT_OFFERED_THIS_COURSE_CLIENT "You have not offered this course. Cannot perform Remove Course. \n"
#define NOT_OFFERED_THIS_COURSE_SERVER "The faculty is not offering this course. Cannot perform Remove Course. \n"
#define OFFERED_ANY_COURSE_CLIENT "You have offered at least 1 course. \n"
#define OFFERED_ANY_COURSE_SERVER "The faculty has offered this course. \n"

#define REMOVED_MAPPING_COURSE_CLIENT "Successfully removed the student -> course mapping. \n"
#define REMOVED_MAPPING_COURSE_SERVER "Removed student -> course mapping. \n"
#define SUCCESSFULLY_REMOVED_COURSE "Successfully removed the course. \n"

#define SUCCESSFULLY_COURSE_DROPPED_CLIENT "You have successfully dropped the course. \n"
#define SUCCESSFULLY_COURSE_DROPPED_SERVER "The student has successfully dropped the course. \n"

#endif