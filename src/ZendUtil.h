#ifndef EXT_SOCKET_SERVER_ZENDUTIL_H
#define EXT_SOCKET_SERVER_ZENDUTIL_H

extern "C" {
#include "php.h"
}

template<typename class_name>
static inline class_name * fetch_from_zend_object(zend_object *obj) {
    return (class_name *)((char *)obj - XtOffsetOf(class_name, std));
}

#endif //EXT_SOCKETSERVER_ZENDUTIL_H