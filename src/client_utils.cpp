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

#include "client_utils.h"

int usage(int status) {
    std::cout
        << "usage: ./client HOST PORT USER\n"
        << "    HOST    Name of the remote host running myftpd\n"
        << "    PORT    Port on which myftpd is listening.\n"
        << "    USER    The name you want to be identified by.\n";
    return status;
}

int _write(int socket_fd, char* message, char error_msg[]) {
    int bytes = write(socket_fd, message, strlen(message));
	if (bytes == -1) {
		error(error_msg);
	}
    return bytes;
}

int _read(int socket_fd, char* message, char error_msg[BUFSIZ]) {
    int bytes = read(socket_fd, message, BUFSIZ);
	if (bytes == -1) {
		error(error_msg);
	}
    message[bytes] = '\0';
    return bytes;
}

void error(char *fmt, ...) {
    va_list args;
    char msg[BUFSIZ], *prefix = "ERROR: ";
    strcpy(msg, prefix);

    va_start(args, fmt);
    vsprintf(msg+strlen(prefix), fmt, args);
    va_end(args);

    perror(msg);
    exit(EXIT_FAILURE);
}