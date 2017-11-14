
#pragma once

int _write(int socket_fd, char* message, char error_msg[]);

int _read(int socket_fd, char* message, char error_msg[BUFSIZ]);

