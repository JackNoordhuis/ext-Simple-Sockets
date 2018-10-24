<?php
/**
 * Generated stub file for code completion purposes
 */

namespace jacknoordhuis\socketserver {
class SocketServer{

	public function __construct(int $domain, int $type, int $protocol, string $address, int $port){}

	public function option(int $level, int $opt_name, int $opt_val) : void{}

	public function bind() : void{}

	public function listen(int $backlog = null) : void{}

	public function accept() : \jacknoordhuis\socketserver\SocketClient{}

	public function select() : \jacknoordhuis\socketserver\SocketClient{}

	public function read_from(int $bytes, &$address, &$port) : string{}

	public function send_to(string $bytes, string $address, int $port) : string{}

	public function shutdown(int $how) : void{}

	public function close() : void{}

	public function closed() : bool{}

	public function __destruct(){}
}

class SocketClient{

	public function __construct(){}

	public function read() : string{}

	public function write(string $bytes) : void{}

	public function shutdown(int $how) : void{}

	public function close() : void{}

	public function closed() : bool{}

	public function __destruct(){}
}
}
