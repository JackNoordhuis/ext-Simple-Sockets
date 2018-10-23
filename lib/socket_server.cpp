//
// Created by Jack Noordhuis on 18/8/18.
//

#include "socket_server.h"
#include "socket_server_errors.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>

SocketServer::SocketServer(int domain, int type, int protocol, const char *address, unsigned short port) {
    _addr.sin_family = (u_int8_t)domain;
    _addr.sin_port = htons(port);
    if(inet_aton(address, &_addr.sin_addr) == 0) {
        throw socket_server::string_runtime(socket_server::errors::UNABLE_TO_RESOLVE_ADDRESS);
    }

    _sockfd = socket(domain, type, protocol);

    if(_sockfd < 0) {
        throw socket_server::errno_runtime(socket_server::errors::CREATE_SOCKET_ERROR);
    }

    FD_ZERO(&_fd_info.master);
    FD_ZERO(&_fd_info.read_fds);
}

SocketServer::~SocketServer() {
    if(!_closed) {
        close();
    }
}

int SocketServer::option(int level, int optname, const void *optval) {
    int result = setsockopt(_sockfd, level, optname, optval, sizeof(optval));

    if(result < 0) { // allow custom socket options to return their own values
        throw socket_server::errno_runtime(socket_server::errors::UNABLE_TO_SET_SOCKET_OPT);
    }

    return result; // allow custom socket options to return their own values
}

void SocketServer::bind() {
    if(::bind(_sockfd, (struct sockaddr*) &_addr, sizeof(_addr)) < 0) {
        throw socket_server::errno_runtime(socket_server::errors::UNABLE_TO_BIND_TO_SOCKET);
    }

    _closed = false;
}

void SocketServer::listen(int backlog) {
    if(::listen(_sockfd, backlog) < 0) {
        throw socket_server::errno_runtime(socket_server::errors::UNABLE_TO_LISTEN_TO_SOCKET);
    }

    FD_SET(_sockfd, &_fd_info.master);
    _fd_info.fdmax = _sockfd;
}

SocketClient* SocketServer::accept() {
    int len = sizeof(_addr);
    int fd = ::accept(_sockfd, (struct sockaddr*) &_addr, (socklen_t*) &len);

    if(fd < 0) {
        throw socket_server::errno_runtime(socket_server::errors::UNABLE_TO_ACCEPT);
    }

    return new SocketClient(fd);
}

SocketClient* SocketServer::select() {
    if(::select(_fd_info.fdmax + 1, &_fd_info.master, nullptr, nullptr, nullptr) < 0) {
        throw socket_server::errno_runtime(socket_server::errors::UNABLE_TO_SELECT);
    }

    return accept();
}

void SocketServer::shutdown(int how) {
    if(how == SHUT_RD and !_accept_reads) { // check if we're attempting to disable reads and the're already disabled
        throw socket_server::string_runtime(socket_server::errors::SHUTDOWN_DISABLE_DISABLED_READS);
    } else if(how == SHUT_WR and !_accept_writes) { // check if we're attempting to disable writes and the're already disabled
        throw socket_server::string_runtime(socket_server::errors::SHUTDOWN_DISABLE_DISABLED_WRITES);
    } else if(how == SHUT_RDWR and (!_accept_reads or !_accept_writes)) { // check if we're attempting to disable reads and/or and either are already disabled
        if(!SocketServer::_accept_reads and !_accept_writes) {
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

void SocketServer::close() {
    if(::close(_sockfd) != 0) { // error
        throw socket_server::errno_runtime(socket_server::errors::UNABLE_TO_CLOSE_SOCKET);
    }

    _closed = true;
}