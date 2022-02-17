# string

A dynamic string library for C

This library defines the struct `String`, which is meant to be used to handle heap allocated strings.

# Dependencies

This library depends on my [string view library](https://github.com/nikos-alexandris/str_view),
and adds it as a submodule.

# Quick Start

Clone the repo in your project

```shell
git clone --recursive https://github.com/nikos-alexandris/string
```

or add it as a submodule in a git project

```shell
git submodule add https://github.com/nikos-alexandris/string
```

and simply include the `string.h` header

 ```c
 #include "string/string.h"
 ```