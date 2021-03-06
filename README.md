# string

A dynamic string library for C

This library defines the struct `String`, which is meant to be used to handle heap allocated strings.

# Handling errors

`String`s are dynamically allocated, which means that almost every operation on them
can cause an error through `malloc`, `realloc` etc. When en error occurs, the returned
or modified `String` will have a special form, for which you can check by using
`string_is_null(String)`. To check if a function can fail, refer to its documentation
in the header file `string.h`.

# StrView

This library uses my string view library, which defines many useful functions
for the `StrView` struct. To remove code duplication, these functions have not
been ported for `String`, because you can just use `string_to_sv(String)` and
get a view of the whole string, and then use those functions on the view. The
String module is focused on providing functions that modify dynamically allocated
strings, whereas the StrView module is focused on providing functions that operate
on constant strings.

# Dependencies

This library depends on [my string view library](https://github.com/nikos-alexandris/str_view).

# Quick Start

Clone the str_view repo and this repo into the same directory

```shell
git clone https://github.com/nikos-alexandris/str_view
git clone https://github.com/nikos-alexandris/string
```

and simply include the `string.h` header

 ```c
 #include "string/string.h"
 ```