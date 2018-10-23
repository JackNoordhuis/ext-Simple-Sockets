//
// Created by Jack Noordhuis on 18/8/18.
//

#ifndef EXT_SOCKET_SERVER_SOCKET_SERVER_H
#define EXT_SOCKET_SERVER_SOCKET_SERVER_H

#include "socket_client.h"
#include <netinet/in.h>
#include <sys/socket.h>

struct fd_info {
    fd_set master;    // master file descriptor list
    fd_set read_fds;  // temp file descriptor list for select()
    int fdmax;        // maximum file descriptor number
};

class SocketServer {
private:
    int _sockfd;

    struct sockaddr_in _addr;

    bool _accept_reads = false;
    bool _accept_writes = false;

    bool _closed = true;
public:

    struct fd_info _fd_info;

    /**
     * Constructor
     */
    SocketServer(int domain, int type, int protocol, const char *address, unsigned short port);

    /**
     * Destructor
     *
     * Automatically closes the socket if still open
     */
    ~SocketServer();

    /**
     * Set an option for the socket.
     *
     * @param level The level at which to apply the opt (usually SOL_SOCKET)
     * @param optname The bit-shifted int of option constants from <sys/socket.h>
     * @param optval Usually 1 or NULL
     *
     * @throws Relevant network error if unsuccessful
     *
     * @return Zero on success or another value specified by a custom socket option
     */
    int option(int level, int optname, const void *optval);

    /**
     * Attempt to bind to the address given in the constructor
     *
     * @throws Relevant network error if unsuccessful
     */
    void bind();

    /**
     * Mark the socket as passive (able to accept incoming connections)
     *
     * @param backlog Maximum number of pending connections to queue for accept, if the queue
     * exceeds this number the client may receive an error with an indication of ECONNREFUSED.
     *
     * @throws Relevant network error if unsuccessful
     */
    void listen(int backlog);

    /**
     * Accept a new connection from the connection pool
     *
     * @throws Relevant network error if unsuccessful
     *
     * @return A new tcp_client instance
     */
    SocketClient* accept();

    /**
     *
     */
    SocketClient* select();

    /**
     * Disallow reading and/or writing to the socket (also stops incoming connections).
     *
     * @param how Disallow reading (also stops incoming connections) and/or writing to the socket
     *
     * @throws Relevant network error if unsuccessful
     */
    void shutdown(int how = SHUT_RD);

    /**
     * Close the socket (not open for new connections)
     */
    void close();

    /**
     * Check if the socket has been closed.
     *
     * @return If the socket has been closed
     */
    bool closed() {
        return _closed;
    }

};


#endif //EXT_SOCKET_SERVER_SOCKET_SERVER_H