#ifndef STRING_H
#define STRING_H

#include <stdio.h>
#include "str_view/str_view.h"

typedef struct String {
	char *buffer;
	size_t size;
	size_t capacity;
} String;

#define STRING_FMT "%.*s"
#define STRING_ARG(s) (int)(s).size, (s).buffer

/*
 * Returns an empty String.
 *
 * The empty string is not allocated.
 */
String
string_empty();

/*
 * Constructs a String from a null terminated
 * c string.
 *
 * Returns -1 on error; 0 on success and stores the result in s.
 */
int
string_from_cstr(const char *cs, String *s);

/*
 * Constructs a String from a stream and a delimiter.
 * The resulting String does not contain the delimiter.
 *
 * Returns -1 on error; 0 on success and stores the result in s.
 */
int
string_from_stream(FILE *stream, int delim, String *s);

/*
 * Constructs a String from an StrView.
 *
 * Returns -1 on error; 0 on success and stores the result in s.
 */
int
string_from_sv(StrView sv, String *s);

/*
 * Constructs a String from a formatted string.
 *
 * For example,
 *
 * ```
 * String s;
 * (void)string_from_fmt(&s, "Hello, %s!", "world");
 * ```
 */
int
string_from_fmt(String *s, const char *template, ...);

/*
 * Same as string_from_fmt but takes the variadic args as a va_list
 */
int
string_from_vfmt(String *s, const char *template, va_list args);

/*
 * Copies dst to src.
 *
 * Both strings need to be freed separately.
 */
int
string_copy(String src, String *dst);

void
string_move(String *src, String *dst);

/*
 * Constructs an StrView from s. If s is later freed, the
 * resulting StrView becomes invalidated
 */
StrView
string_to_sv(String s);

int
string_concat(String s1, String s2, String *dst);

/*
 * Frees the memory occupied by s
 */
void
string_free(String *s);

/*
 * Checks if string is empty. This means that
 */
bool
string_is_empty(String s);

int
string_push(String *s, char c);

int
string_pop(String *s, char *c);

int
string_append(String *s, String src);

#endif /* STRING_H */