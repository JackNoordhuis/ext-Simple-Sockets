//
// Created by Jack Noordhuis on 22/10/18.
//

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "src/PhpSocketServer.h"
#include "src/PhpSocketClient.h"

extern "C" {
#include "php.h"
#include "ext/standard/info.h"
#include "php_socket_server.h" //this one has to be C always, so the engine can understand it
}

/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(socket_server)
{
    register_socket_server_class();
    register_socket_client_class();
    return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(socket_server)
{
    php_info_print_table_start();
    php_info_print_table_header(2, "socket_server support", "enabled");
    php_info_print_table_end();

}
/* }}} */

/* {{{ socket_server_module_entry
 */
zend_module_entry socket_server_module_entry = {
        STANDARD_MODULE_HEADER,
        "socket_server",				    /* Extension name */
        NULL,				                /* zend_function_entry */
        PHP_MINIT(socket_server),		    /* PHP_MINIT - Module initialization */
        NULL,	                            /* PHP_MSHUTDOWN - Module shutdown */
        NULL,				                /* PHP_RINIT - Request initialization */
        NULL,								/* PHP_RSHUTDOWN - Request shutdown */
        PHP_MINFO(socket_server),		/* PHP_MINFO - Module info */
        PHP_SOCKET_SERVER_VERSION,		/* Version */
        STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_SOCKET_SERVER
extern "C" {
#ifdef ZTS
ZEND_TSRMLS_CACHE_DEFINE()
#endif
ZEND_GET_MODULE(socket_server)
}
#endif