# escape
This is a library for C (and C++) which provides functions for printing ANSI escape sequences, colored output, formatted colored output and moving the cursor around. It is a library that I actually use.

If you don't know anything about escape sequences, read [this wikipedia article](https://en.wikipedia.org/wiki/ANSI_escape_code). If you want a list of most of the ANSI, I used [this github gist](https://gist.github.com/fnky/458719343aabd01cfb17a3a4f7296797) as help for this library.

## Installation
To compile, run:
```sh
make
```
Or, if you only want the dynamic or the static version, run
```sh
make shared
```
or
```sh
make static
```
respectively.

To install, run (as root):
```sh
make install
```
By default the library is installed at `/usr/local/lib`, which might not be sourced by the linker. If that is the case, you can run (as root):
```sh
ldconfig /usr/local/lib
```

## Usage
To use in C, include the `<escape.h>` header and link to the library. To do this in gcc or clang compile with `-lescape`. If using the static version, use `-l:libescape.a` instead.

The library mainly provides `printf()`-like functions that parse directives starting with `$`, like `$c1` for the color red, `$c3` for green or `$b` for bold. A list of these directives and a description of all the functions can be found in the header file. Here is an very basic example:
```c
#include <escape.h>

int main() {
    // This function just parses '$' directives.
    // Prints "Hello" in red and "world!" in green.
    printc("$c1Hello, $c3world!\n");

    // This calls printf() first and then printc().
    // Prints "Hello" normally and "world!" in bold.
    printfc("Hello, $b%s!\n", "world");

    return 0;
}
```

More examples can be found in `example/`

There is support for C++ through the `esc` namespace, e.g `esc::printc()`.

## TODO
By far the biggest problem is that it's not possible to turn the functionality off at runtime and there is nothing implemented to prevent colored output on terminals that don't support it ([haha](https://twoot.site/@bean/113056942625234032)).

Not all escape sequences were implemented (yet). Those that I would use are though.

Performance is a bit of an issue. It's not really bad but it could be way better. Right now, functions like `printc()` parse the given string at runtime even tho all the necessary information is known at compile time. It would be nice to have zero overhead but it is a hard problem to solve.
