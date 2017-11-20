#include "utils.h"
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>  //inet_addr
#include <unistd.h>
#include <iostream>
#include <sstream>      // sstream


int _write(int socket_fd, char* message, char error_msg[]) {
    //printf("SENT: %s\n", message);
    int bytes = write(socket_fd, message, strlen(message));
	if (bytes == -1) {
		error(error_msg);
	}
    return bytes;
}

int _read(int socket_fd, char* message, char error_msg[BUFSIZ]) {
	bzero(message, BUFSIZ);
    int bytes = read(socket_fd, message, BUFSIZ);
	if (bytes == -1) {
		error(error_msg);
	}
    message[bytes] = '\0';
    //printf("READ: %s\n", message);
    return bytes;
}

int usage(int status) {
    std::cout
        << "usage: ./client HOST PORT USER\n"
        << "    HOST    Name of the remote host running chat server.\n"
        << "    PORT    Port on which chat server is listening.\n"
        << "    USER    The name you want to be identified by.\n";
    return status;
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
