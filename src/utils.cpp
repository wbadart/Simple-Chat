#include "utils.h"

int _write(int socket_fd, char* message, char error_msg[]) {
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
    return bytes;
}