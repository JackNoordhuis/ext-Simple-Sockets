//
// Created by Jack Noordhuis on 22/10/18.
//

#include "lib/socket_client.h"
#include "ZendUtil.h"

#include "PhpSocketClient.h"

extern "C" {
#include "php.h"
#include "Zend/zend_exceptions.h"
#include "ext/spl/spl_exceptions.h"
}

zend_class_entry *socket_client_entry;
zend_object_handlers socket_client_handlers;

/* internal object handlers */
zend_object* socket_client_new(zend_class_entry *class_type) {
    socket_client_obj *object = (socket_client_obj *)ecalloc(1, sizeof(socket_client_obj) + zend_object_properties_size(class_type));//NOLINT

    zend_object_std_init(&object->std, class_type);
    object_properties_init(&object->std, class_type);

    object->std.handlers = &socket_client_handlers;

    return &object->std;
}

void socket_client_free(zend_object *obj) {
    socket_client_obj *object = fetch_from_zend_object<socket_client_obj>(obj);//NOLINT
    delete object->container;

    //call the standard free handler
    zend_object_std_dtor(&object->std);
}

/* PHP-land SocketClient methods */

ZEND_BEGIN_ARG_INFO_EX(arginfo_SocketClient___construct, 0, 0, 0)
ZEND_END_ARG_INFO()

PHP_METHOD(PhpSocketClient, __construct) {
    zend_parse_parameters_none_throw();
    zend_throw_exception_ex(spl_ce_BadMethodCallException, 0, "Cannot explicitly create a socket client object!");
}

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_SocketClient_read, 0, 0, IS_STRING, 0)
ZEND_END_ARG_INFO()

PHP_METHOD(PhpSocketClient, read) {
    zend_parse_parameters_none_throw();
    socket_client_obj *intern = fetch_from_zend_object<socket_client_obj>(Z_OBJ_P(getThis()));//NOLINT

    std::string bytes;

    try {
        bytes = intern->container->read();
    } catch (std::exception &e) {
        zend_throw_exception_ex(spl_ce_RuntimeException, 0, e.what());
        return;
    }

    RETURN_STRING(bytes.c_str());
}

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_SocketClient_write, 0, 1, IS_VOID, 0)
    ZEND_ARG_TYPE_INFO(0, bytes, IS_STRING, 0)
ZEND_END_ARG_INFO()

PHP_METHOD(PhpSocketClient, write) {
    zend_string *bytes;

    ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 1, 1)
            Z_PARAM_STR(bytes)
    ZEND_PARSE_PARAMETERS_END();

    socket_client_obj *intern = fetch_from_zend_object<socket_client_obj>(Z_OBJ_P(getThis()));//NOLINT

    try {
        intern->container->write(std::string(&bytes->val[0]));
    } catch (std::exception &e) {
        zend_throw_exception_ex(spl_ce_RuntimeException, 0, e.what());
    }
}

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_SocketClient_shutdown, 0, 1, IS_VOID, 0)
                ZEND_ARG_TYPE_INFO(0, how, IS_LONG, 0)
ZEND_END_ARG_INFO()

PHP_METHOD(PhpSocketClient, shutdown) {
    zend_long how;

    ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 0, 1)
            Z_PARAM_LONG(how)
    ZEND_PARSE_PARAMETERS_END();

    socket_client_obj *intern = fetch_from_zend_object<socket_client_obj>(Z_OBJ_P(getThis()));//NOLINT

    try {
        intern->container->shutdown((int)how);
    } catch(std::exception &e) {
        zend_throw_exception_ex(spl_ce_RuntimeException, 0, e.what());
        return;
    }
}

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO(arginfo_SocketClient_close, IS_VOID, 0)
ZEND_END_ARG_INFO()

PHP_METHOD(PhpSocketClient, close) {
    zend_parse_parameters_none_throw();

    socket_client_obj *intern = fetch_from_zend_object<socket_client_obj>(Z_OBJ_P(getThis()));//NOLINT

    try {
        if(!intern->container->closed()) {
            intern->container->close();
        }
    } catch(std::exception &e) {
        zend_throw_exception_ex(spl_ce_RuntimeException, 0, e.what());
        return;
    }
}

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO(arginfo_SocketClient_closed, _IS_BOOL, 0)
ZEND_END_ARG_INFO()

PHP_METHOD(PhpSocketClient, closed) {
    zend_parse_parameters_none_throw();

    socket_client_obj *intern = fetch_from_zend_object<socket_client_obj>(Z_OBJ_P(getThis()));//NOLINT

    RETURN_BOOL(intern->container->closed());
}

ZEND_BEGIN_ARG_INFO(arginfo_SocketClient___destruct, 0)
ZEND_END_ARG_INFO()

PHP_METHOD(PhpSocketClient, __destruct) {
    zend_parse_parameters_none_throw();

    socket_client_obj *intern = fetch_from_zend_object<socket_client_obj>(Z_OBJ_P(getThis()));//NOLINT

    try {
        if(!intern->container->closed()) {
            intern->container->close();
        }
    } catch(std::exception &e) {
        zend_throw_exception_ex(spl_ce_RuntimeException, 0, e.what());
        return;
    }
}

zend_function_entry socket_client_class_methods[] = {
        PHP_ME(PhpSocketClient, __construct, arginfo_SocketClient___construct, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
        PHP_ME(PhpSocketClient, read, arginfo_SocketClient_read, ZEND_ACC_PUBLIC)
        PHP_ME(PhpSocketClient, write, arginfo_SocketClient_write, ZEND_ACC_PUBLIC)
        PHP_ME(PhpSocketClient, shutdown, arginfo_SocketClient_shutdown, ZEND_ACC_PUBLIC)
        PHP_ME(PhpSocketClient, close, arginfo_SocketClient_close, ZEND_ACC_PUBLIC)
        PHP_ME(PhpSocketClient, closed, arginfo_SocketClient_closed, ZEND_ACC_PUBLIC)
        PHP_ME(PhpSocketClient, __destruct, arginfo_SocketClient___destruct, ZEND_ACC_PUBLIC | ZEND_ACC_DTOR)
        PHP_FE_END
};

void register_socket_client_class() {
    memcpy(&socket_client_handlers, zend_get_std_object_handlers(), sizeof(socket_client_handlers));
    socket_client_handlers.offset = (int)XtOffsetOf(socket_client_obj, std);
    socket_client_handlers.free_obj = socket_client_free;

    zend_class_entry ce;
    INIT_CLASS_ENTRY(ce, "jacknoordhuis\\socketserver\\SocketClient", socket_client_class_methods);
    ce.create_object = socket_client_new;
    socket_client_entry = zend_register_internal_class(&ce);
}