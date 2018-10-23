ext-simple-sockets
===================
__A (crappy) PHP Extension developed by [Jack Noordhuis](https://twitter.com/JackNoordhuis)__

### About
Simple Sockets is an experimental PHP extension written in C++ to mess around with an object wrapper around the
unix sockets API, it is not guaranteed to work, probably doesn't follow the 'PHP extension best practises' and I
will not provide ongoing support for its use nor provided any guarantee on the quality of implementation.

### Compiling
Simple Sockets currently only supports unix systems and no support for windows is planned. The extension has
only been tested on macOS High Sierra 10.13.6 so feel free to submit any fixes for your platform if you run into
any problems.

The compilation process is the stock standard PHP extension compilation:
```bash
phpize && ./configure
```

You can then run `make install` to install the library to your PHP installations extension folder then simply
add `extension=path/to/simple_sockets.so` to enable the extension.

### Developing
If you're using an IDE like PHPStorm you can add the stubs directory to your include path for auto-completion.
You can find example uses of the `jacknoordhuis\simplesockets\SocketServer` class in the [examples](examples)
directory along with example code for consuming the servers data using the standard PHP socket extensions methods.

### Licence information
The content of this repo is licensed under the GNU Lesser General Public License (GPL) v3. A full copy of the license
is available [here](LICENSE).

>This program is free software: you can redistribute it and/or modify<br/>
>it under the terms of the GNU General Public License as published by<br/>
>the Free Software Foundation, either version 3 of the License, or<br/>
>(at your option) any later version.<br/>
>
>This program is distributed in the hope that it will be useful,<br/>
>but WITHOUT ANY WARRANTY; without even the implied warranty of<br/>
>MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the<br/>
>GNU General Public License for more details.<br/>
>
>You should have received a copy of the GNU General Public License<br/>
>along with this program.  If not, see http://www.gnu.org/licenses/