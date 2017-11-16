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

#pragma once

#include <unistd.h>     // close
#include <sys/types.h>  // PF_INET, SOCK_STREAM
#include <netdb.h>      // hostent, gethostbyname
#include <sstream>      // sstream
#include <sys/time.h>   // gettimeofday
#include <iostream>     // cout
#include <string.h>     // strcmp
#include "utils.h"

int login(int socket_fd, char* user_name);

void* handle_message(void*);

int private_message(int);

int broadcast_message(int);

void print_prompt();