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

	return 1;
}

void* handle_message(void* socket_fd) {
	char msg_buffer[BUFSIZ];
	int socket = *(int*)socket_fd;
	int last_char;

	while (ACTIVE) {
		_read(socket, msg_buffer, "Failed to listen for messages");
		last_char = strlen(msg_buffer) - 1;

		// check what kind of message it is
		if (msg_buffer[last_char] == 'D') { 
			msg_buffer[last_char] = '\0';
			std::cout << std::endl << "    ####### New Message: Message From: " 
				<< msg_buffer << " ####### " << std::endl;
			print_prompt();
		} else if (msg_buffer[last_char] == 'C') {
			if (msg_buffer[last_char-1] == '0') {
				// successful transaction message
				std::cout << "Message Sent" << std::endl;
				print_prompt();
			} else if (msg_buffer[last_char-1] == '1') {
				// private exchange
				msg_buffer[last_char-1] = '\0';
				send_private_message(msg_buffer, socket);
			} else if (msg_buffer[last_char-1] == '2') {
				// broadcast exchange
				puts("i am here");
				send_broadcast_message(socket);
			} 
			/*else if (msg_buffer[last_char-1] == '3') {
				// login new user
				send_password_new(socket);
			} else if (msg_buffer[last_char-1] == '4') {
				// login old user
				send_password_old(socket);
			} else if (msg_buffer[last_char-1] == '5') {
				// login password success
			} else if (msg_buffer[last_char-1] == '6') {
				// login password fail
				std::cout << "Incorrect Password" << std::endl <<
					"Try Again ";
				send_password_old(socket);
			}
			*/
		}
	}
	return 0;
}

int send_private_message(char users[BUFSIZ], int socket) {
	char msg_buffer[BUFSIZ];
	char username[BUFSIZ];
	// print online users
	std::cout << users << std::endl;

	// read username
	std::cout << "Enter Username >> ";
	std::cin >> username;

	// send user
	_write(socket, username, "Failed to send username");
	// read message
	fgets(msg_buffer, BUFSIZ, stdin);
	// send message
	_write(socket, msg_buffer, "Failed to send private message");

	return 1;
}

int send_broadcast_message(int socket) {
	printf("Enter Broadcast Message >> ");
	fflush(stdout);

	char msg_buffer[BUFSIZ];
	// read message
	fgets(msg_buffer, BUFSIZ, stdin);
	// std::cin.getline(msg_buffer, BUFSIZ);
	printf("%s", msg_buffer);
	// send message
	_write(socket, msg_buffer, "Failed to send broadcast message");

	return 1;
}


int private_message(int socket_fd) {
	char msg_buffer[BUFSIZ];

	strcpy(msg_buffer, "P");
	_write(socket_fd, msg_buffer, "Failed to send command message P");

	return 1;
}

int broadcast_message(int socket_fd) {
	char msg_buffer[BUFSIZ];

	strcpy(msg_buffer, "B");
	_write(socket_fd, msg_buffer, "Failed to send command message B");

	return 1;
}

void print_prompt() {
	std::cout <<
		"Enter P for a private message" << std::endl <<
		"Enter B for a public message" << std::endl <<
		"Enter E to exit" << std::endl <<
		">> ";
}