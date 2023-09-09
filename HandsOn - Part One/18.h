/*
===============================================================================================
Name : 18.h
Author : Hemant Kumar Chandavar
Description : Write a program to perform Record locking.
a. Implement write lock
b. Implement read lock
Create three records in a file. Whenever you access a particular record, first
lock it then modify/access to avoid race condition.
Date : 28th August, 2023
===============================================================================================
*/

#ifndef TICKET_DB
#define TICKET_DB

struct db
{
	int train_num;
	int ticket_count;
} ticket_db;

#endif
