#ifndef SOCKET_CONSTANTS_H
#define SOCKET_CONSTANTS_H

// Socket Programming Constants
#define NEW_CLIENT "New client connected to server. \n"
#define CLIENT_CONNECTED "\nConnection successful...\n"
#define SOCKET_ERROR "socket() error. \n"
#define BIND_ERROR "bind() error. \n"
#define LISTEN_ERROR "listen() error. \n"
#define CONNECT_ERROR "connect() error. \n"
#define FORK_FAILED "fork() failed. \n"
#define DISCONNECTED_SERVER "\nClient disconnected..."
#define DISCONNECTED_CLIENT "\nYou have successfully logged out. \n"
#define CLIENT_SOCKET_CLOSED "Client Socket is closed. \n"

// Client File and Server File Usage Constants
#define CLIENT_FILE_USAGE "Usage : ./client <ip-address> <port-no> \n"
#define SERVER_FILE_USAGE "Usage : ./server <port-no> \n"

// Reaping Child Process Constants
#define PROCESS_REAPED_UNSUCCESSFUL "Waitpid was unsuccessful. \n"
#define PROCESS_STATUS_NOTAVAILABLE                                            \
	"There was at least one child whose status information could not be "  \
	"obtained. \n"
#define PROCESS_REAPED_SUCCESSFUL "Child Process was successfully reaped. \n"

#define WRITE_UNSUCCESSFUL "Error while sending prompt to the client. \n"
#define WRITE_SUCCESSFUL "Successfully sent prompt to the client. \n"
#define READ_UNSUCCESSFUL "Server could not read the client choice. \n"
#define READ_SUCCESSFUL                                                        \
	"Server was successfully able to read the client choice. \n"
#define READ_NO_CHOICE "Client did not mention his choice. \n"

#define WRITE_ADMIN_PROMPT_SUCCESSFUL                                          \
	"Successfully sent admin prompt to the client. \n"
#define READ_ADMIN_CHOICE_UNSUCCESSFUL                                         \
	"Server could not read the admin choice. \n"
#define ADMIN_INCORRECT_CHOICE_SERVER                                          \
	"Admin has mentioned a choice that does not fall within the given "    \
	"range. \n"
#define ADMIN_INCORRECT_CHOICE_CLIENT                                          \
	"You have mentioned an incorrect choice. \n"
#define ADMIN_NO_CHOICE_SERVER "Admin has not entered any choice. \n"
#define ADMIN_NO_CHOICE_CLIENT "You have not entered any choice. \n"

#define STUDENT_RECORD_WRITE_FAILED "Write Student Record Failed. \n"
#define STUDENT_RECORD_NOTHING_WRITTEN                                         \
	"No Student Record was written to the file. \n"
#define STUDENT_RECORD_WRITE_SUCCESSFUL                                        \
	"Student Record was successfully written to the file. \n"
#define STUDENT_FILE_OPEN_FAILED "Opening Student file failed. \n"
#define STUDENT_RECORD_READ_FAILED "Read Student Record Failed. \n"
#define STUDENT_RECORD_EOF "End of student file reached while reading. \n"
#define STUDENT_RECORD_READ_SUCCESSFUL                                         \
	"Student Record was successfully read from the file. \n"

#define FACULTY_RECORD_WRITE_FAILED "Write Faculty Record Failed. \n"
#define FACULTY_RECORD_NOTHING_WRITTEN                                         \
	"No Faculty Record was written to the file. \n"
#define FACULTY_RECORD_WRITE_SUCCESSFUL                                        \
	"Faculty Record was successfully written to the file. \n"
#define FACULTY_FILE_OPEN_FAILED "Opening Faculty file failed. \n"
#define FACULTY_RECORD_READ_FAILED "Read Faculty Record Failed. \n"
#define FACULTY_RECORD_EOF "End of Faculty file reached while reading. \n"
#define FACULTY_RECORD_READ_SUCCESSFUL                                         \
	"Faculty Record was successfully read from the file. \n"

#endif