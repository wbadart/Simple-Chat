/**
 * src/client/client.cpp
 *
 * Driver program for chat client.
 *
 * Rob Simari
 * Royce Branning
 * Will Badart
 *
 * created: NOV 2017
 **/

#include <iostream>
#include <pthread.h>
#include "client_utils.h"

int READY = 1;
int ACTIVE = 1;
States STATE = States::CMD_CHOICE;

int main(int argc, char *argv[]) {
	// check for correct number of args
	if (argc != 4) {
 		return usage(1);
	}

	// check if host/server exists
	struct hostent* hp;
	hp = gethostbyname(argv[1]);
	if (!hp) {
		error("Host %s cannot be found\n", argv[1]);
	}

	// buffer for sending and receiving messages
	char msg_buffer[BUFSIZ];
	bzero(msg_buffer, BUFSIZ);

	int socket_fd;
	struct sockaddr_in sin;

	bzero((char*)&sin, sizeof(sin));
	sin.sin_family = AF_INET;
	bcopy((char *)hp->h_addr,
	  (char *)&sin.sin_addr.s_addr, hp->h_length);
	sin.sin_port = htons(atoi(argv[2]));

	// create socket
	if ((socket_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		char msg[] = "Failed to create socket\n";
		error(msg);
	}

	// connect()
	if (connect(socket_fd, (struct sockaddr*) &sin, sizeof(sin)) < 0) {
		char msg[] = "Failed to connect to server\n";
		error(msg);
	}

	// sends server the username
	if (login(socket_fd, argv[3]) == 0) {
		char msg[] = "Failed to login\n";
		error(msg);
	}

	std::cout << "Welcome " << argv[3] << "!" << std::endl;
	print_prompt();

	// start listening thread
	pthread_t thread;
	int rc = pthread_create(&thread, NULL, handle_message, (void*)&socket_fd);

	if (rc != 0) {
		error("Could not create thread");
	}

	char cmd[BUFSIZ];

	while (true) {
		if (READY) {
			std::cin >> cmd;
			if (strcmp(cmd, "P") == 0) {
				// send private message
				READY = 0; STATE = States::START_DM;
				start_private_message(socket_fd);
                while(STATE == States::WAIT_FOR_USERS);
                // state is now GET_DM_USERS
				get_dm_username(socket_fd);
				while(STATE == States::GET_DM_USERNAME);
				// state is now GET_DM_BODY
				get_dm_body(socket_fd);

			} else if (strcmp(cmd, "B") == 0) {
				// send broadcast message
				READY = 0; STATE = States::START_BROADCAST;
				start_broadcast_message(socket_fd);
                while(STATE == States::WAIT_BROADCAST_READY);
                // state is now GET_BROADCAST_BODY
                get_broadcast_body(socket_fd);
                while(STATE == States::GET_BROADCAST_BODY);

			} else if (strcmp(cmd, "E") == 0) {
				// shut down thread
				ACTIVE = 0;
				_write(socket_fd, "E", "Failed to send exit command");
				break;

			} else {
				std::cout << "Invalid Command: " << cmd << std::endl;
			}
		}
	}

	close(socket_fd);

    return EXIT_SUCCESS;
}
