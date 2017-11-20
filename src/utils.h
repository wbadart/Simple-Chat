#include <stdio.h>      // fgets, fopen, fseek
#include <stdlib.h>   
#include <stdarg.h>    // va_start, va_end

int _write(int socket_fd, char* message, char error_msg[]);

int _read(int socket_fd, char* message, char error_msg[BUFSIZ]);

int usage(int status);

void error(char* fmt, ...);
