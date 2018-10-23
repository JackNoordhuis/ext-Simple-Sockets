//
// Created by Jack Noordhuis on 19/8/18.
//

#include <string>
#include <sys/socket.h>
#include "socket_client.h"
#include "socket_server_errors.h"

SocketClient::SocketClient(int sockfd) {
    _sockfd = sockfd;
    _accept_reads = true;
    _accept_writes = true;
    _closed = false;
}

SocketClient::~SocketClient() {
    if(!_closed) {
        close();
    }
}

std::string SocketClient::read() {
    char buf[1024];

    ssize_t data = recv(_sockfd, buf, sizeof(buf), 0);

    if(data <= 0) {
        if (data == 0) { // client disconnected
            throw socket_server::string_runtime(socket_server::errors::CLIENT_UNABLE_TO_READ_DISCONNECTED);
        } else { // error
            throw socket_server::errno_runtime(socket_server::errors::CLIENT_UNABLE_TO_READ);
        }
    }

    return std::string(reinterpret_cast<char const*>(buf), (unsigned long) data);
}

void SocketClient::write(std::string payload) {
    char buf[payload.length()];
    std::strcpy(buf, payload.c_str());

    if(send(_sockfd, &buf, (size_t) sizeof(buf), 0) < 0) {
        throw socket_server::errno_runtime(socket_server::errors::CLIENT_UNABLE_TO_WRITE);
    }
}

void SocketClient::shutdown(int how) {
    if(how == SHUT_RD and !_accept_reads) { // check if we're attempting to disable reads and the're already disabled
        throw socket_server::string_runtime(socket_server::errors::SHUTDOWN_DISABLE_DISABLED_READS);
    } else if(how == SHUT_WR and !_accept_writes) { // check if we're attempting to disable writes and the're already disabled
        throw socket_server::string_runtime(socket_server::errors::SHUTDOWN_DISABLE_DISABLED_WRITES);
    } else if(how == SHUT_RDWR and (!_accept_reads or !_accept_writes)) { // check if we're attempting to disable reads and/or and either are already disabled
        if(!_accept_reads and !_accept_writes) {
            throw socket_server::string_runtime(socket_server::errors::SHUTDOWN_DISABLE_READS_WRITES_DISABLED_READS_WRITES);
        } else if(!_accept_reads) {
            throw socket_server::string_runtime(socket_server::errors::SHUTDOWN_DISABLE_READS_WRITES_DISABLED_READS);
        } else if(!_accept_writes) {
            throw socket_server::string_runtime(socket_server::errors::SHUTDOWN_DISABLE_READS_WRITES_DISABLED_WRITES);
        }
    }

    if(::shutdown(_sockfd, how) != 0) { // error
        throw socket_server::errno_runtime(socket_server::errors::UNABLE_TO_SHUTDOWN_SOCKET);
    }

    if(how == SHUT_RD) {
        _accept_reads = false;
    } else if(how == SHUT_WR) {
        _accept_writes = false;
    } else if(how == SHUT_RDWR) {
        _accept_reads = false;
        _accept_writes = false;
    }
}

void SocketClient::close() {
    if(::close(_sockfd) != 0) { // error
        throw socket_server::errno_runtime(socket_server::errors::UNABLE_TO_CLOSE_SOCKET);
    }

    _closed = true;
}