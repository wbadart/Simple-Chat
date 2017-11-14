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

// the thread function
//void *connection_handler(void *);

int main(int argc, char *argv[]) {
    UserDatabase db("./users.db");

    int socket_desc, client_socket, c;
    struct sock_addr_in server, client;

    // Create socket
    socket_desc = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_desc == -1){
        printf("Could not create socket");
    }
    puts("Socket created");

    // Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons( 8888 );

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
    //puts("Waiting for incoming connections...");
    //c = sizeof(struct sockaddr_in);

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
        if( db._valid_username(client_usrname) && strcmp(db.query(client_usrname), "") != 0){
        	// send response of Old/New
        	_write(client_socket, "Old", "Failed to send response");
        	// get password
        	_read(client_socket, client_pass, "Failed to get password");
            while(!db.login(client_usrname, client_pass)){
	        	_write(client_socket, "Fail", "Failed to send response");
                printf("wrong password");
                _read(client_socket, client_pass, "Failed to get password");
            }
            // login successful
        	_write(client_socket, "Success", "Failed to send response");
            printf("returning user signed in");
        } else {
        	_write(client_socket, "New", "Failed to send response");
        	_read(client_socket, client_pass, "Failed to get new password");
	        if(db.add_user(client_usrname, client_pass)){
                printf("user created and signed in");
                _write(client_socket, "Success", "Failed to send success message");
            } else {
                perror("add_user failed");
                return 1;
            }
        }
        

        // if( pthread_create( &thread_id, NULL, connection_handler, (void*) &client_socket) < 0){
        //     perror("could not create thread");
        //     return 1;
        // }

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

/*
void *connection_handler(void *socket_desc){
    // Get the socket descriptor
    int sock  *(int*)socket_desc;
    int read_size;
    char *message, client_message[2000];

    //Send some messages to the client
    message = "Greetings! I am your connection handler\n";
    write(sock, message, strlen(message));

    message = "Now type something and I shall repeat what you type \n";
    write(sock, message, strlen(message));

    // Receive a message from client
    while( (read_size = recv(sock, client_message, 2000, 0)) > 0){
        // end of string marker
        client_message[read_size] = '\0';
        
        // Send the message back to client
        write(sock, client_message, strlen(client_message));

        // clear the message buffer
        memset(client_message, 0, 2000);
    }

    if(read_size == 0){
        puts("Client disconnected");
        fflush(stdout);
    } else if( read_size == -1){
        perror("recv failed");
    }

    return 0;
}
*/
