//
// Created by Jack Noordhuis on 20/8/18.
//

#ifndef EXT_SOCKET_SERVER_SOCKET_SERVER_ERRORS_H
#define EXT_SOCKET_SERVER_SOCKET_SERVER_ERRORS_H


#include <stdexcept>
#include <string>
#include <errno.h>
#include <boost/format.hpp>

namespace socket_server {
    namespace errors {

        const std::string UNABLE_TO_RESOLVE_ADDRESS = "Unable to resolve address!";
        const std::string CREATE_SOCKET_ERROR = "Unable to create socket: [%1%] %2%";
        const std::string UNABLE_TO_SET_SOCKET_OPT = "Unable to set socket option: [%1%] %2%";
        const std::string UNABLE_TO_BIND_TO_SOCKET = "Unable to bind to socket: [%1%] %2%";
        const std::string UNABLE_TO_LISTEN_TO_SOCKET = "Unable to listen to socket: [%1%] %2%";
        const std::string UNABLE_TO_ACCEPT = "Unable to accept from socket: [%1%] %2%";
        const std::string UNABLE_TO_SELECT = "Unable to select from socket: [%1%] %2%";
        const std::string SHUTDOWN_DISABLE_DISABLED_READS = "Attempted to disable reads when they're already disabled!";
        const std::string SHUTDOWN_DISABLE_DISABLED_WRITES = "Attempted to disable writes when they're already disabled!";
        const std::string SHUTDOWN_DISABLE_READS_WRITES_DISABLED_READS_WRITES = "Attempted to disable reads and writes when reads and writes are both already disabled!";
        const std::string SHUTDOWN_DISABLE_READS_WRITES_DISABLED_READS = "Attempted to disable reads and writes when reads are already disabled!";
        const std::string SHUTDOWN_DISABLE_READS_WRITES_DISABLED_WRITES = "Attempted to disable reads and writes when writes are already disabled!";
        const std::string UNABLE_TO_SHUTDOWN_SOCKET = "Unable to shutdown socket: [%1%] %2%";
        const std::string UNABLE_TO_CLOSE_SOCKET = "Unable to close socket: [%1%] %2%";

        const std::string CLIENT_UNABLE_TO_READ_DISCONNECTED = "Cannot read from client due to client disconnected!";
        const std::string CLIENT_UNABLE_TO_READ = "Unable to read from client: [%1%] %2%";
        const std::string CLIENT_UNABLE_TO_WRITE = "Unable to write to client: [%1%] %2%";
    }

    static std::runtime_error string_runtime(const std::string &message) {
        return std::runtime_error(message);
    }

    static std::runtime_error errno_runtime(const std::string &base) {
        return std::runtime_error(boost::str(boost::format(base.c_str()) % errno % std::strerror(errno)));
    }
}


#endif //EXT_SOCKET_SERVER_SOCKET_SERVER_ERRORS_H