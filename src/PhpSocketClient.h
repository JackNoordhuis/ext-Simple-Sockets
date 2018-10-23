//
// Created by Jack Noordhuis on 22/10/18.
//

#ifndef EXT_SOCKET_SERVER_PHPSOCKETCLIENT_H
#define EXT_SOCKET_SERVER_PHPSOCKETCLIENT_H

#include "lib/socket_client.h"

extern "C" {
#include "php.h"
}

PHP_METHOD(PhpSocketServer, __construct);
PHP_METHOD(PhpSocketServer, read);
PHP_METHOD(PhpSocketServer, write);
PHP_METHOD(PhpSocketServer, shutdown);
PHP_METHOD(PhpSocketServer, close);
PHP_METHOD(PhpSocketServer, closed);
PHP_METHOD(PhpSocketServer, __destruct);

extern zend_class_entry *socket_client_entry;

void register_socket_client_class();

typedef struct {
    SocketClient *container;
    zend_object std;
} socket_client_obj;

#endif //EXT_SOCKET_SERVER_PHPSOCKETCLIENT_H
