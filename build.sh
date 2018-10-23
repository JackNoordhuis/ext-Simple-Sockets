#!/usr/bin/env bash

#
# This is a personalised build script, it will not work on your system without
# the correct configuration and is only used to streamline development.
#

phpize && ./configure && make install
make clean && phpize --clean
rm -f stubs/socket_server.php && extension-stub socket_server >> stubs/socket_server.php