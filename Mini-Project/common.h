#ifndef COMMON_H
#define COMMON_H

#include "course.h"
#include "course_student_mapping.h"
#include "faculty.h"
#include "server_constants.h"
#include "student.h"

// Set of functions that are commonly used in all admin, faculty and student files.
int read_student_record( int fileDescriptor, struct student* record, int record_indx, int record_size );
int read_faculty_record( int fileDescriptor, struct faculty* record, int record_indx, int record_size );
int read_course_record( int fileDescriptor, struct course* record, int record_indx, int record_size );
int read_mapping_record( int fileDescriptor, struct mapping* record, int record_indx, int record_size );

void write_student_record( int fileDescriptor, struct student* record, int record_indx, int record_size, int flag );
void write_faculty_record( int fileDescriptor, struct faculty* record, int record_indx, int record_size, int flag );
void write_course_record( int fileDescriptor, struct course* record, int record_indx, int record_size, int flag );
void write_mapping_record( int fileDescriptor, struct mapping* record, int record_indx, int record_size, int flag );

int isRowEmpty( const char array[][COURSEID_LENGTH], int row );
int isStructEmpty( const struct course* c );
void saveVariablesToFile();
void loadVariablesFromFile();

#endif