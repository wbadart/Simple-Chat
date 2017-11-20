/**
 * src/server.cpp
 *
 * Driver program for chat server.
 *
 * Rob Simari
 * Royce Branning
 * Will Badart
 *
 * created: NOV 2017
 **/

#include <iostream>
#include <userdatabase.h>
#include <stdio.h>
#include <string.h>     //strlen
#include <stdlib.h>     //strlen
#include <netinet/in.h>
#include <netdb.h>
#include <sys/socket.h>
#include <arpa/inet.h>  //inet_addr
#include <unistd.h>     //write
#include <pthread.h>    //for threading , link with lpthread
#include "utils.h"

#define MAX_THREADS 10

std::map<char*, int> online_users;
UserDatabase db("./users.db");


// the thread function
void *connection_handler(void *);

int main(int argc, char *argv[]) {
    if(argc < 2) {
        puts("Missing argument PORT.");
        exit(1);
    }

    int socket_desc, client_socket, c;
    struct sockaddr_in server, client;

    // Create socket
    socket_desc = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_desc == -1){
        printf("Could not create socket");
    }
    puts("Socket created");

    // Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(atoi(argv[1]));

    // Bind
    if( bind(socket_desc, (struct sockaddr *)&server, sizeof(server)) < 0){
        //print the error message
        perror("bind failed. Error");
        return 1;
    }
    puts("bind done");

    // Listen
    listen(socket_desc , 3);

    // Accept incoming connection
    puts("Waiting for incoming connections...");
    c = sizeof(struct sockaddr_in);
    pthread_t thread_id;

    while( (client_socket = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c)) ){
        puts("Connection accepted");

        // authenticate user
        char client_usrname[BUFSIZ];
        _read(client_socket, client_usrname, "Failed to get username");

        char client_pass[BUFSIZ];
        std::string res = db.query(client_usrname);
        if(strcmp(res.c_str(), "") != 0){
            // send response of Old/New
            _write(client_socket, "Old", "Failed to send response");
            // get password
            _read(client_socket, client_pass, "Failed to get password");
            while(!db.login(client_usrname, client_pass, client_socket)){
                _write(client_socket, "Fail", "Failed to send response");
                printf("wrong password");
                _read(client_socket, client_pass, "Failed to get password");
            }
            // login successful
            _write(client_socket, "Success", "Failed to send response");
            printf("returning user signed in\n");
        } else {
            _write(client_socket, "New", "Failed to send response");
            _read(client_socket, client_pass, "Failed to get new password");
            if(db.add_user(client_usrname, client_pass)){
                printf("user created and signed in");
                _write(client_socket, "Success", "Failed to send success message");
                db.login(client_usrname, client_pass, client_socket);
            } else {
                perror("add_user failed");
                return 1;
            }
        }


        if( pthread_create( &thread_id, NULL, connection_handler, (void*) &client_socket) < 0){
            perror("could not create thread");
            return 1;
        }

        //Now join the thread, so that we don't terminate before the thread
        // pthread_join( thread_id, NULL);
        puts("Handler assigned");
    }

    if ( client_socket < 0){
        perror("accept failed");
        return 1;
    }

    return 0;
}

/*
 * This will handle connection for each client
 * */
void *connection_handler(void *socket_desc){
    // Get the socket descriptor
    int sock = *(int*)socket_desc;
    char message[BUFSIZ], client_message[BUFSIZ];

    // Receive a message from client
    while( _read(sock, client_message, "Failed to read from client") > 0) {
        std::cout << client_message << std::endl;
        if (strcmp(client_message, "P") == 0) {
			printf("PRIVATE\n");
        	
            // put C1 in front of message
			strcpy(message, "C1");

            // send back live users (from db class)
            for(const auto& it: db.client_sockets()) {
                strcat(message, it.first.c_str());
                strcat(message, "\n");
                std::cout << it.first << ":" << it.second << std::endl;
            }

			printf("users: %s\n", message);
		
			_write(sock, message, "write back live users to client failed");
            printf("users: %s\n", message);


            // read username from client
            _read(sock, client_message, "Failed to read username from client");
            int rec_sock = db.client_sockets().at(client_message);

			// read message from client
			_read(sock, client_message, "Failed to read message from client");
			printf("%d %d ", rec_sock, sock);

			// send message to correct socket
			char enc_client_message[BUFSIZ];
            strcpy(enc_client_message,"D");
            strcat(enc_client_message, client_message);

			_write(rec_sock, enc_client_message, "Failed to send private message");

			strcpy(client_message, "C0");
			_write(sock, client_message, "Failed to confirm message");

        } else if (strcmp(client_message, "B") == 0) {
            printf("BROADCAST\n");

			// send back C2 code
			char broadcast_mess[BUFSIZ];
			strcpy(broadcast_mess, "C2");
			_write(sock, broadcast_mess, "Failed to write back to client");

            // read message from client
            _read(sock, client_message, "Failed to receive broadcast message from client");

			// send message to all other clients
			char enc_client_message[BUFSIZ];
            strcpy(enc_client_message,"D");
            strcat(enc_client_message, client_message);

            for(const auto& it: db.client_sockets())
                if(it.second != sock)
					_write(it.second, enc_client_message, "Failed to broadcast message");

			strcpy(broadcast_mess, "C0");
			_write(sock, broadcast_mess, "Failed to send final message to client");

        } else if (strcmp(client_message, "E") == 0) {
            // remove client from online_users
            for(const auto& it : db.client_sockets())
                if( it.second == sock)
                    db.logout(it.first);

            puts("Client disconnected");
            return 0;
        }
        // write(sock, client_message, strlen(client_message));
    }

    return 0;
}

