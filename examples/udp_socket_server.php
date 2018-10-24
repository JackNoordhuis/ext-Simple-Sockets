<?php

/* Get the port for the service. */
$service_port = 8649;

/* Get the IP address for the target host. */
$address = "127.0.0.1";

echo "Starting simple UDP server..." . PHP_EOL;

$server = null;
$client = null;

try {
    $server = new \jacknoordhuis\socketserver\SocketServer(AF_INET, SOCK_DGRAM, SOL_UDP, "0.0.0.0", 8649);

    $server->option(SOL_SOCKET, SO_REUSEADDR, 1);

    echo "Attempting to bind to '$address' on port '$service_port'..." . PHP_EOL;
    $server->bind();

} catch(\RuntimeException $e) {
    echo "Error whilst starting socket server!" . PHP_EOL;
    echo $e->getMessage() . PHP_EOL;

    $server = null;
}

if($server !== null) {
    try {
        echo "Waiting for connection..." . PHP_EOL;

        $address = "";
        $port = 0;
        $bytes = $server->read_from(1024, $address, $port);
        echo PHP_EOL . "Read '" . $bytes . "' (" . strlen($bytes) . " bytes)" . PHP_EOL;

        if ($bytes === "PING") {
            $server->send_to($payload = "PONG", $address, $port);
            echo "Wrote '" . $payload . "' (" . strlen($payload) . " bytes)" . PHP_EOL . PHP_EOL;
        }

        echo "Disconnecting client..." . PHP_EOL;
    } catch (\RuntimeException $e) {
        echo "Error whilst accepting incoming connection!" . PHP_EOL;
        echo $e->getMessage() . PHP_EOL;
    }

    echo "Stopping TCP server..." . PHP_EOL;

    if(!$server->closed()) {
        $server->close();
    }
}