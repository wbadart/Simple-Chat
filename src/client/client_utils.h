/**
 * src/client/client_utils.cpp
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
#include "../utils.h"

#define CONTROL_CHAR1(s) (s[0])
#define CONTROL_CHAR2(s) (s[1])

#define STRIP_CONTROL_CHAR1(s) (s+1)
#define STRIP_CONTROL_CHAR2(s) (s+2)

enum class States {
    CMD_CHOICE,
    GET_BROADCAST_BODY,
    START_DM,
    START_BROADCAST,
    GET_DM_USERNAME,
    WAIT_FOR_USERS,
    GET_DM_BODY,
    WAIT_DM_READY,
    WAIT_BROADCAST_READY,
};

extern int ACTIVE;
extern int READY;
extern States STATE;

int login(int socket_fd, char* user_name);

void* handle_message(void*);

void get_dm_username(int);

void get_dm_body(int);

void get_broadcast_body(int);

int start_private_message(int);

int start_broadcast_message(int);

void print_prompt();
