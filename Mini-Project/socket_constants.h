#ifndef SOCKET_CONSTANTS_H
#define SOCKET_CONSTANTS_H

// Socket Programming Constants
#define INPUT_MESSAGE "Input message (q or Q to exit) : "
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
#define WAITPID_UNSUCCESSFUL "Waitpid was unsuccessful. \n"
#define WAITPID_STATUSNOTAVAILABLE                                             \
	"There was at least one child whose status information could not be "  \
	"obtained. \n"
#define PROCESS_REAPED_SUCCESS "Child Process was successfully reaped. \n"

#endif