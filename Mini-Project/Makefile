# -*- Makefile -*-

all: server client

server: server.o admin.o student.o faculty.o
	gcc server.o admin.o student.o faculty.o -o server

server.o: server.c admin.h faculty.h student.h server_constants.h socket_constants.h
	gcc -c server.c

admin.o: admin.c admin.h server_constants.h faculty.h socket_constants.h student.h
	gcc -c admin.c

student.o: student.c student.h
	gcc -c student.c

faculty.o: faculty.c faculty.h course.h
	gcc -c faculty.c

client: client.o socket_constants.h
	gcc client.o -lpthread -D_REENTRANT -o client

client.o: client.c
	gcc -c client.c

clean:
	rm *.o *.txt server client