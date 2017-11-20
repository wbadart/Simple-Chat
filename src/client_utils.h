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
#include <string>
#include "utils.h"

extern int ACTIVE;
extern int READY;
extern int STATE;

int login(int socket_fd, char* user_name);

void* handle_message(void*);

int send_private_message(int);

int send_broadcast_message(int);

int private_message(int);

int broadcast_message(int);

void print_prompt();
