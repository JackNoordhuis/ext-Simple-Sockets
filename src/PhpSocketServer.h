//
// Created by Jack Noordhuis on 22/10/18.
//

#ifndef EXT_SOCKET_SERVER_PHPSOCKETSERVER_H
#define EXT_SOCKET_SERVER_PHPSOCKETSERVER_H

#include "lib/socket_server.h"

extern "C" {
#include "php.h"
}

PHP_METHOD(PhpSocketServer, __construct);
PHP_METHOD(PhpSocketServer, option);
PHP_METHOD(PhpSocketServer, bind);
PHP_METHOD(PhpSocketServer, listen);
PHP_METHOD(PhpSocketServer, accept);
PHP_METHOD(PhpSocketServer, select);
PHP_METHOD(PhpSocketServer, shutdown);
PHP_METHOD(PhpSocketServer, close);
PHP_METHOD(PhpSocketServer, closed);
PHP_METHOD(PhpSocketServer, __destruct);

extern zend_class_entry *socket_server_entry;

void register_socket_server_class();

typedef struct {
    SocketServer *container;
    zend_object std;
} socket_server_obj;

#endif //EXT_SOCKET_SERVER_PHPSOCKETSERVER_H
