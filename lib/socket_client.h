//
// Created by Jack Noordhuis on 19/8/18.
//

#ifndef EXT_SOCKET_SERVER_SOCKET_CLIENT_H
#define EXT_SOCKET_SERVER_SOCKET_CLIENT_H

#include <string>
#include <sys/socket.h>

class SocketClient {
private:
    int _sockfd;

    bool _accept_reads = false;
    bool _accept_writes = false;

    bool _closed = true;
public:

    /**
     * Constructor
     *
     * @param sockfd The file descriptor for the socket
     */
    explicit SocketClient(int sockfd);

    /**
     * Destructor
     */
    ~SocketClient();

    /**
     * @throws Relevant error if unsuccessful
     *
     * @return Payload struct containing number of bytes read and the bytes
     */
    std::string read();

    /**
     * Writes the specified payload to the socket
     *
     * @throws Relevant error if unsuccessful
     */
    void write(std::string);

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


#endif //EXT_SOCKET_SERVER_SOCKET_CLIENT_H
