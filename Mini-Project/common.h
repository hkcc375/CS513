#ifndef COMMON_H
#define COMMON_H

#include "course.h"
#include "faculty.h"
#include "student.h"

int read_student_record( int fileDescriptor, struct student* record,
                         int record_indx, int record_size );
void read_faculty_record( int fileDescriptor, struct faculty* record,
                          int record_indx, int record_size );

void write_student_record( int fileDescriptor, struct student* record,
                           int record_indx, int record_size, int flag );
void write_faculty_record( int fileDescriptor, struct faculty* record,
                           int record_indx, int record_size, int flag );
void write_course_record( int fileDescriptor, struct course* record,
                          int record_indx, int record_size, int flag );

#endif