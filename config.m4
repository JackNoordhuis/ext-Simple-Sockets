dnl $Id$
dnl config.m4 for extension socket_server

PHP_ARG_ENABLE(socket_server, whether to enable socket_server support,
[  --enable-socket-server           Enable Socket Server extension])

if test "$PHP_SOCKET_SERVER" != "no"; then
  PHP_REQUIRE_CXX()

  dnl apparently PHP_REQUIRE_CXX() isn't good enough if we're compiling as a shared library
  PHP_ADD_LIBRARY(stdc++, 1, SIMPLE_TCP_SERVER_SHARED_LIBADD)
  PHP_SUBST(SOCKET_SERVER_SHARED_LIBADD)

  PHP_NEW_EXTENSION(socket_server, socket_server.cpp lib/socket_server.cpp lib/socket_client.cpp src/PhpSocketServer.cpp src/PhpSocketClient.cpp, $ext_shared,, -DZEND_ENABLE_STATIC_TSRMLS_CACHE=1 -std=c++11)
  PHP_ADD_BUILD_DIR($ext_builddir/src, 1)
  PHP_ADD_BUILD_DIR($ext_builddir/lib, 1)
  PHP_ADD_INCLUDE($ext_builddir)
fi