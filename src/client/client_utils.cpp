/**
 * src/client/client_utils.cpp
 *
 * Implementation of client helper functions.
 *
 * Rob Simari
 * Royce Branning
 * Will Badart
 *
 * created: NOV 2017
 **/

#include "../utils.h"
#include "client_utils.h"

int login(int socket_fd, char* username) {
	char msg_buffer[BUFSIZ];

	// send server username
	_write(socket_fd, username, "Failed to send username");

	// wait for confirmation
	_read(socket_fd, msg_buffer, "Failed to receive confirmation");

	char password[BUFSIZ];

	if (strcmp(msg_buffer, "New") == 0) {
		std::cout << "New User? Create Password >> ";
		std::cin >> password;
	} else if (strcmp(msg_buffer, "Old") == 0) {
		std::cout << "Welcome Back! Enter Password >> ";
		std::cin >> password;
	} else {
		return 0;
	}

	// send password
	_write(socket_fd, password, "Failed to login");
	// wait for confirmation fo password
	_read(socket_fd, msg_buffer, "Could not read response from login");

	if (strcmp(msg_buffer, "Success") == 0) {
		return 1;
	} else if (strcmp(msg_buffer, "Fail") == 0) {
		while (true) {
			std::cout << "Incorrect Password: Please Enter Again >> ";
			std::cin >> password;
			// send password
			_write(socket_fd, password, "Failed to login");
			// wait for confirmation fo password
			_read(socket_fd, msg_buffer, "Could not read response from login");
			if (strcmp(msg_buffer, "Success") == 0) return 1;
		}
	}

	return 1;
}

void* handle_message(void* socket_fd) {
	char msg_buffer[BUFSIZ];
	int socket = *(int*)socket_fd;

	while (ACTIVE) {
		_read(socket, msg_buffer, "Failed to listen for messages");
		if (strlen(msg_buffer) <= 0) break;

		// check what kind of message it is
		if (CONTROL_CHAR1(msg_buffer) == 'D') {

			std::cout << std::endl << "    ####### New Message: "
				<< STRIP_CONTROL_CHAR1(msg_buffer) << " ####### " << std::endl;
            print_prompt();

		} else if (CONTROL_CHAR1(msg_buffer) == 'C') {

			if (CONTROL_CHAR2(msg_buffer) == '0') {
				// successful transaction message
				std::cout << "Message Sent" << std::endl;
				print_prompt();
				READY = 1; STATE = States::CMD_CHOICE;

			} else if (CONTROL_CHAR2(msg_buffer) == '1') {
        // print list of online users and prepare to get name of user
				std::cout << "USERS:\n" << STRIP_CONTROL_CHAR2(msg_buffer) << std::endl;
                STATE = States::GET_DM_USERNAME;
                print_prompt();

			} else if (CONTROL_CHAR2(msg_buffer) == '2') {
				// prompt user for broadcast message input
                STATE = States::GET_BROADCAST_BODY;
                print_prompt();
			}

		}
	}
	return 0;
}

void get_dm_username(int socket) {
	char username[BUFSIZ];

	// read username
	std::cin >> username;

	// send user
	_write(socket, username, "Failed to send username");
    STATE = States::GET_DM_BODY;	
}

void get_dm_body(int socket) {
	char msg_buffer[BUFSIZ];

	print_prompt();
	// read message
	std::cin.ignore();
	std::cin.getline(msg_buffer, BUFSIZ);
	// send message
	_write(socket, msg_buffer, "Failed to send private message");

    STATE = States::CMD_CHOICE;
}

void get_broadcast_body(int socket) {
	char msg_buffer[BUFSIZ];
	// read message
	std::cin.ignore();
	std::cin.getline(msg_buffer, BUFSIZ);

	// send message
	_write(socket, msg_buffer, "Failed to send broadcast message");
    STATE = States::CMD_CHOICE;	
}

int start_private_message(int socket_fd) {
	char msg_buffer[BUFSIZ];

	strcpy(msg_buffer, "P");
	_write(socket_fd, msg_buffer, "Failed to send command message P");
	// waiting for the users to get sent back
    STATE = States::WAIT_FOR_USERS;

	return 1;
}

int start_broadcast_message(int socket_fd) {
	char msg_buffer[BUFSIZ];

	strcpy(msg_buffer, "B");
	_write(socket_fd, msg_buffer, "Failed to send command message B");
    STATE = States::WAIT_BROADCAST_READY;

	return 1;
}

void print_prompt() {
	    switch(STATE) {
        case States::CMD_CHOICE:
            printf("Enter P for a private message\n");
	        printf("Enter B for a broadcast message\n");
	        printf("Enter E to exit\n");
            printf(">> ");
            break;
        case States::GET_BROADCAST_BODY:
            printf("Enter Broadcast Message >> ");
            break;
        case States::GET_DM_USERNAME:
            printf("Enter Username >> ");
            break;
        case States::GET_DM_BODY:
            printf("Enter Private Message >> ");
            break;
        case States::WAIT_BROADCAST_READY:
            break;
        case States::WAIT_DM_READY:
            break;
        default:
            error("UNKNOWN STATE");

    }
    fflush(stdout);
}
