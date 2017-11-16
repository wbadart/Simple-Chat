/**
 * src/client.cpp
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

	const char prompt[] = ">> ";

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

	if (login(socket_fd, argv[3]) == 0) {
		char msg[] = "Failed to login\n";
		error(msg);
	}

	// start listening thread
	pthread_t thread;
	int rc = pthread_create(&thread, NULL, handle_message, (void*)&socket_fd);
	
	std::cout << "Welcome " << argv[3] << "!" << std::endl;

	char cmd[BUFSIZ];

	while (true) {
		print_prompt();
		std::cin >> cmd; // ?? WTF this doesnt work
		std::cout << cmd;

		if (strcmp(cmd, "P") == 0) {
			private_message(socket_fd);
		} else if (strcmp(cmd, "B") == 0) {
			broadcast_message(socket_fd);
		} else if (strcmp(cmd, "E") == 0) {
			pthread_join(thread, NULL);
			break;
		} else {
			std::cout << "Invalid Command: " << cmd << std::endl;
		}
	}

	close(socket_fd);

    return EXIT_SUCCESS;
}
