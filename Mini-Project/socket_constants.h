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
#define CLIENT_DISCONNECTED "\nClient disconnected..."

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
#define READ_ADMIN_CHOICE_SUCCESSFUL                                           \
	"Server was successfully able to read the admin choice. \n"
#define READ_ADMIN_CHOICE_UNSUCCESSFUL                                         \
	"Server could not read the admin choice. \n"
#define READ_ADMIN_NO_CHOICE "Admin did not mention his choice. \n"

#endif