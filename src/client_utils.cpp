/**
 * src/client_utils.cpp
 *
 * Implementation of client helper functions.
 *
 * Rob Simari
 * Royce Branning
 * Will Badart
 *
 * created: NOV 2017
 **/

#include "utils.h"
#include "client_utils.h"

int login(int socket_fd, char* username) {
	char msg_buffer[BUFSIZ];
		
	// send server username
	_write(socket_fd, username, "Failed to send username");

	// wait for confirmation
	_read(socket_fd, msg_buffer, "Failed to receive confirmation");

	char password[BUFSIZ];
	std::cout << msg_buffer << std::endl;

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
			_write(socket_fd, "Failed to login", password);
			// wait for confirmation fo password
			_read(socket_fd, msg_buffer, "Could not read response from login");
			if (strcmp(msg_buffer, "Success") == 0) return 1;
		}
	}

	return 0;
}

void* handle_message(void* socket_fd) {
	char msg_buffer[BUFSIZ];
	int socket = *(int*)socket_fd;

	while (true) {
		_read(socket, msg_buffer, "Failed to listen for messages");
		// check what kind of message it is
		std::cout << std::endl << "    ####### New Message: Message From :" 
			<< msg_buffer << " ####### " << std::endl;
		print_prompt();
	}
	return 0;
}

int private_message(int socket_fd) {
	char msg_buffer[BUFSIZ];
	char username[BUFSIZ];

	strcpy(msg_buffer, "P");
	_write(socket_fd, msg_buffer, "Failed to send command message P");
	_read(socket_fd, msg_buffer, "Failed to get online users");
	bzero(msg_buffer, BUFSIZ);

	std::cout << msg_buffer << std::endl;
	std::cout << "Enter Username >> ";
	std::cin >> username;
	_write(socket_fd, username, "Failed to send username");

	// wait for response?

	std::cout << std::endl << "Enter Message >> ";
	fgets(msg_buffer, BUFSIZ, stdin);
	msg_buffer[strlen(msg_buffer)-1] = '\0';
	
	_write(socket_fd, msg_buffer, "Failed to send message");

	std::cout << "Message Sent" << std::endl;

	return 1;
}

int broadcast_message(int socket_fd) {
	char msg_buffer[BUFSIZ];

	strcpy(msg_buffer, "B");
	_write(socket_fd, msg_buffer, "Failed to send command message B");
	bzero(msg_buffer, BUFSIZ);

	std::cout << std::endl << "Enter Message >> ";

	fgets(msg_buffer, BUFSIZ, stdin);
	msg_buffer[strlen(msg_buffer)-1] = '\0';
	_write(socket_fd, msg_buffer, "Failed to send message");

	std::cout << "Message Sent" << std::endl;

	return 1;
}

void print_prompt() {
	std::cout <<
		"Enter P for a private message" << std::endl <<
		"Enter B for a public message" << std::endl <<
		"Enter E to exit" << std::endl <<
		">> ";
}