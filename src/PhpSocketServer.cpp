//
// Created by Jack Noordhuis on 22/10/18.
//

#include "lib/socket_server.h"
#include "ZendUtil.h"

#include "PhpSocketClient.h"
#include "PhpSocketServer.h"

extern "C" {
#include "php.h"
#include "Zend/zend_exceptions.h"
#include "Zend/zend_smart_str.h"
#include "ext/spl/spl_exceptions.h"
}

zend_class_entry *socket_server_entry;
zend_object_handlers socket_server_handlers;

/* internal object handlers */

zend_object* socket_server_new(zend_class_entry *class_type) {
    socket_server_obj *object = (socket_server_obj *)ecalloc(1, sizeof(socket_server_obj) + zend_object_properties_size(class_type));//NOLINT

    zend_object_std_init(&object->std, class_type);
    object_properties_init(&object->std, class_type);

    object->std.handlers = &socket_server_handlers;

    return &object->std;
}

void socket_server_free(zend_object *obj) {
    socket_server_obj *object = fetch_from_zend_object<socket_server_obj>(obj);//NOLINT
    delete object->container;

    //call the standard free handler
    zend_object_std_dtor(&object->std);
}

/* PHP-land SocketServer methods */

ZEND_BEGIN_ARG_INFO_EX(arginfo_SocketServer___construct, 0, 0, 5)
    ZEND_ARG_TYPE_INFO(0, domain, IS_LONG, 0)
    ZEND_ARG_TYPE_INFO(0, type, IS_LONG, 0)
    ZEND_ARG_TYPE_INFO(0, protocol, IS_LONG, 0)
    ZEND_ARG_TYPE_INFO(0, address, IS_STRING, 0)
    ZEND_ARG_TYPE_INFO(0, port, IS_LONG, 0)
ZEND_END_ARG_INFO()

PHP_METHOD(PhpSocketServer, __construct) {
    zend_long domain, type, protocol, port;
    zend_string *address;

    ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 5, 5)
        Z_PARAM_LONG(domain)
        Z_PARAM_LONG(type)
        Z_PARAM_LONG(protocol)
        Z_PARAM_STR(address)
        Z_PARAM_LONG(port)
    ZEND_PARSE_PARAMETERS_END();

    socket_server_obj *intern = fetch_from_zend_object<socket_server_obj>(Z_OBJ_P(getThis()));//NOLINT
    try {
        intern->container = new SocketServer((int)domain, (int)type, (int)protocol, ZSTR_VAL(address), (unsigned short)port);
    } catch(std::exception &e) {
        zend_throw_exception_ex(spl_ce_RuntimeException, 0, e.what());
    }
}


ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_SocketServer_option, 0, 3, IS_VOID, 0)
    ZEND_ARG_TYPE_INFO(0, level, IS_LONG, 0)
    ZEND_ARG_TYPE_INFO(0, opt_name, IS_LONG, 0)
    ZEND_ARG_TYPE_INFO(0, opt_val, IS_LONG, 0)
ZEND_END_ARG_INFO()

PHP_METHOD(PhpSocketServer, option) {
    zend_long level, opt_name, opt_val;

    ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 3, 3)
        Z_PARAM_LONG(level)
        Z_PARAM_LONG(opt_name)
        Z_PARAM_LONG(opt_val)
    ZEND_PARSE_PARAMETERS_END();

    socket_server_obj *intern = fetch_from_zend_object<socket_server_obj>(Z_OBJ_P(getThis()));//NOLINT

    try {
        intern->container->option((int)level, (int)opt_name, (const void *)&opt_val);
    } catch(std::exception &e) {
        zend_throw_exception_ex(spl_ce_RuntimeException, 0, e.what());
    }
}

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO(arginfo_SocketServer_bind, IS_VOID, 0)
ZEND_END_ARG_INFO()

PHP_METHOD(PhpSocketServer, bind) {
    zend_parse_parameters_none_throw();

    socket_server_obj *intern = fetch_from_zend_object<socket_server_obj>(Z_OBJ_P(getThis()));//NOLINT

    try {
        intern->container->bind();
    } catch(std::exception &e) {
        zend_throw_exception_ex(spl_ce_RuntimeException, 0, e.what());
    }
}

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_SocketServer_listen, 0, 0, IS_VOID, 0)
    ZEND_ARG_TYPE_INFO(0, backlog, IS_LONG, 0)
ZEND_END_ARG_INFO()

PHP_METHOD(PhpSocketServer, listen) {
    zend_long backlog = 5;

    ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 0, 1)
        Z_PARAM_OPTIONAL
        Z_PARAM_LONG(backlog)
    ZEND_PARSE_PARAMETERS_END();

    socket_server_obj *intern = fetch_from_zend_object<socket_server_obj>(Z_OBJ_P(getThis()));//NOLINT

    try {
        intern->container->listen((int)backlog);
    } catch(std::exception &e) {
        zend_throw_exception_ex(spl_ce_RuntimeException, 0, e.what());
    }
}

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO(arginfo_SocketServer_accept, jacknoordhuis\\socketserver\\SocketClient, 0)
ZEND_END_ARG_INFO()

PHP_METHOD(PhpSocketServer, accept) {
    zend_parse_parameters_none_throw();

    socket_server_obj *intern = fetch_from_zend_object<socket_server_obj>(Z_OBJ_P(getThis()));//NOLINT

    SocketClient *client;
    try {
        client = intern->container->accept();
    } catch(std::exception &e) {
        zend_throw_exception_ex(spl_ce_RuntimeException, 0, e.what());
        return;
    }

    object_init_ex(return_value, socket_client_entry);
    socket_client_obj *client_intern = fetch_from_zend_object<socket_client_obj>(Z_OBJ_P(return_value));//NOLINT

    client_intern->container = client;
}

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO(arginfo_SocketServer_select, jacknoordhuis\\socketserver\\SocketClient, 0)
ZEND_END_ARG_INFO()

PHP_METHOD(PhpSocketServer, select) {
    zend_parse_parameters_none_throw();

    socket_server_obj *intern = fetch_from_zend_object<socket_server_obj>(Z_OBJ_P(getThis()));//NOLINT

    SocketClient *client;
    try {
        client = intern->container->select();
    } catch(std::exception &e) {
        zend_throw_exception_ex(spl_ce_RuntimeException, 0, e.what());
        return;
    }

    object_init_ex(return_value, socket_client_entry);
    socket_client_obj *client_intern = fetch_from_zend_object<socket_client_obj>(Z_OBJ_P(return_value));//NOLINT

    client_intern->container = client;
}

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_SocketServer_read_from, 0, 3, IS_STRING, 0)
    ZEND_ARG_TYPE_INFO(0, bytes, IS_LONG, 0)
    ZEND_ARG_INFO(1, address)
    ZEND_ARG_INFO(1, port)
ZEND_END_ARG_INFO()

PHP_METHOD(PhpSocketServer, read_from) {
    zend_long bytes;
    zval *address, *port = NULL;

    if (zend_parse_parameters_ex(ZEND_PARSE_PARAMS_THROW, ZEND_NUM_ARGS() TSRMLS_CC, "lz/z/", &bytes, &address, &port) == FAILURE)
    {
        return;
    }

    socket_server_obj *intern = fetch_from_zend_object<socket_server_obj>(Z_OBJ_P(getThis()));//NOLINT

    std::string addr;
    unsigned short por;

    try {
        RETVAL_STRING(intern->container->read_from((int)bytes, addr, por).c_str());
    } catch(std::exception &e) {
        zend_throw_exception_ex(spl_ce_RuntimeException, 0, e.what());
        return;
    }

    zval_ptr_dtor(address);
    zval_ptr_dtor(port);

    ZVAL_STRING(address, addr.c_str());
    ZVAL_LONG(port, por);
}

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_SocketServer_send_to, 0, 3, IS_STRING, 0)
    ZEND_ARG_TYPE_INFO(0, bytes, IS_STRING, 0)
    ZEND_ARG_TYPE_INFO(0, address, IS_STRING, 0)
    ZEND_ARG_TYPE_INFO(0, port, IS_LONG, 0)
ZEND_END_ARG_INFO()

PHP_METHOD(PhpSocketServer, send_to) {
    zend_string *bytes, *address;
    zend_long port;

    ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 3, 3)
        Z_PARAM_STR(bytes)
        Z_PARAM_STR(address)
        Z_PARAM_LONG(port)
    ZEND_PARSE_PARAMETERS_END();

    socket_server_obj *intern = fetch_from_zend_object<socket_server_obj>(Z_OBJ_P(getThis()));//NOLINT

    try {
        intern->container->send_to((const char *)&bytes->val[0], (const char *)&address->val[0], (unsigned short)port);
    } catch(std::exception &e) {
        zend_throw_exception_ex(spl_ce_RuntimeException, 0, e.what());
        return;
    }
}

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_SocketServer_shutdown, 0, 1, IS_VOID, 0)
    ZEND_ARG_TYPE_INFO(0, how, IS_LONG, 0)
ZEND_END_ARG_INFO()

PHP_METHOD(PhpSocketServer, shutdown) {
    zend_long how;

    ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 0, 1)
        Z_PARAM_LONG(how)
    ZEND_PARSE_PARAMETERS_END();

    socket_server_obj *intern = fetch_from_zend_object<socket_server_obj>(Z_OBJ_P(getThis()));//NOLINT

    try {
        intern->container->shutdown((int)how);
    } catch(std::exception &e) {
        zend_throw_exception_ex(spl_ce_RuntimeException, 0, e.what());
        return;
    }
}

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO(arginfo_SocketServer_close, IS_VOID, 0)
ZEND_END_ARG_INFO()

PHP_METHOD(PhpSocketServer, close) {
    zend_parse_parameters_none_throw();

    socket_server_obj *intern = fetch_from_zend_object<socket_server_obj>(Z_OBJ_P(getThis()));//NOLINT

    try {
        if(!intern->container->closed()) {
            intern->container->close();
        }
    } catch(std::exception &e) {
        zend_throw_exception_ex(spl_ce_RuntimeException, 0, e.what());
        return;
    }
}

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO(arginfo_SocketServer_closed, _IS_BOOL, 0)
ZEND_END_ARG_INFO()

PHP_METHOD(PhpSocketServer, closed) {
    zend_parse_parameters_none_throw();

    socket_server_obj *intern = fetch_from_zend_object<socket_server_obj>(Z_OBJ_P(getThis()));//NOLINT

    RETURN_BOOL(intern->container->closed());
}

ZEND_BEGIN_ARG_INFO(arginfo_SocketServer___destruct, 0)
ZEND_END_ARG_INFO()

PHP_METHOD(PhpSocketServer, __destruct) {
    zend_parse_parameters_none_throw();

    socket_server_obj *intern = fetch_from_zend_object<socket_server_obj>(Z_OBJ_P(getThis()));//NOLINT

    try {
        if(!intern->container->closed()) {
            intern->container->close();
        }
    } catch(std::exception &e) {
        zend_throw_exception_ex(spl_ce_RuntimeException, 0, e.what());
        return;
    }
}

zend_function_entry socket_server_class_methods[] = {
        PHP_ME(PhpSocketServer, __construct, arginfo_SocketServer___construct, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
        PHP_ME(PhpSocketServer, option, arginfo_SocketServer_option, ZEND_ACC_PUBLIC)
        PHP_ME(PhpSocketServer, bind, arginfo_SocketServer_bind, ZEND_ACC_PUBLIC)
        PHP_ME(PhpSocketServer, listen, arginfo_SocketServer_listen, ZEND_ACC_PUBLIC)
        PHP_ME(PhpSocketServer, accept, arginfo_SocketServer_accept, ZEND_ACC_PUBLIC)
        PHP_ME(PhpSocketServer, select, arginfo_SocketServer_select, ZEND_ACC_PUBLIC)
        PHP_ME(PhpSocketServer, read_from, arginfo_SocketServer_read_from, ZEND_ACC_PUBLIC)
        PHP_ME(PhpSocketServer, send_to, arginfo_SocketServer_send_to, ZEND_ACC_PUBLIC)
        PHP_ME(PhpSocketServer, shutdown, arginfo_SocketServer_shutdown, ZEND_ACC_PUBLIC)
        PHP_ME(PhpSocketServer, close, arginfo_SocketServer_close, ZEND_ACC_PUBLIC)
        PHP_ME(PhpSocketServer, closed, arginfo_SocketServer_closed, ZEND_ACC_PUBLIC)
        PHP_ME(PhpSocketServer, __destruct, arginfo_SocketServer___destruct, ZEND_ACC_PUBLIC | ZEND_ACC_DTOR)
        PHP_FE_END
};

void register_socket_server_class() {
    memcpy(&socket_server_handlers, zend_get_std_object_handlers(), sizeof(socket_server_handlers));
    socket_server_handlers.offset = (int)XtOffsetOf(socket_server_obj, std);
    socket_server_handlers.free_obj = socket_server_free;

    zend_class_entry ce;
    INIT_CLASS_ENTRY(ce, "jacknoordhuis\\socketserver\\SocketServer", socket_server_class_methods);
    ce.create_object = socket_server_new;
    socket_server_entry = zend_register_internal_class(&ce);
}