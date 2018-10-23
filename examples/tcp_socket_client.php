<?php

/* Get the port for the service. */
$service_port = 8649;

/* Get the IP address for the target host. */
$address = "127.0.0.1";

echo "Starting simple TCP client..." . PHP_EOL;

$socket = socket_create(AF_INET, SOCK_STREAM, SOL_TCP);
if($socket === false) {
    echo "socket_create() failed: reason: " . socket_strerror(socket_last_error()) . PHP_EOL;
    exit;
}

echo "Attempting to connect to '$address' on port '$service_port'..." . PHP_EOL;
$result = socket_connect($socket, $address, $service_port);
if ($result === false) {
    echo "socket_connect() failed. " . PHP_EOL . "Reason: ($result) " . socket_strerror(socket_last_error($socket)) . PHP_EOL;
}

$payload = "PING";
socket_write($socket, $payload, strlen($payload));
echo PHP_EOL . "Wrote '" . $payload . "' (" . strlen($payload) . " bytes)" . PHP_EOL;

while ($payload = socket_read($socket, 1024)) {
    echo "Read '" . $payload . "' (" . strlen($payload) . " bytes)" . PHP_EOL . PHP_EOL;
}

echo "Closing socket..." . PHP_EOL;
socket_close($socket);