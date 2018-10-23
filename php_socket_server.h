//
// Created by Jack Noordhuis on 22/10/18.
//

#ifndef PHP_SOCKET_SERVER_H
#define PHP_SOCKET_SERVER_H

#include "php.h"

extern zend_module_entry socket_server_module_entry;
#define phpext_socket_server_ptr &socket_server_module_entry

#define PHP_SOCKET_SERVER_VERSION "0.1.0"

#ifdef ZTS
#include "TSRM.h"
#endif

#if defined(ZTS) && defined(COMPILE_DL_SOCKET_SERVER)
ZEND_TSRMLS_CACHE_EXTERN()
#endif

#endif //PHP_SOCKET_SERVER_H
