# Simple-Chat
Simple chat server and client.

**Colaborators:**

- Rob Simari <rsimari>
- Royce Branning <rbrannin>
- Will Badart <wbadart>


## Building

To build the whole program, client and server, run `make`. The
Makefile provides rules for building the client and the server
individually via `make client` and `make server`.


## Running

The server binary can be run with the following options:

```
$ ./bin/server [PORT]
usage: server PORT
    PORT        Port on which to listen for incoming connections.
```

And the client should be run as follows:

```
$ ./bin/client --help
usage: client HOST PORT <username>
    HOST        Name of the remote host running chat server
    PORT        Port on which chat server is listening.
    <username>  username of existing or new user.
```

## Included Files

