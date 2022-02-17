#ifndef STRING_H
#define STRING_H

#include <stdio.h>
#include "str_view/str_view.h"

typedef struct String {
	char *__buffer;
	size_t __size;
	size_t __capacity;
} String;

#define STRING_FMT SV_FMT
#define STRING_ARG(s) SV_ARG((s).sv)

/*
 * Returns an empty String.
 *
 * The empty string is not allocated.
 */
String
string_empty();

/*
 * Constructs a String from a null terminated
 * c string and stores it in s.
 *
 * Returns -1 on error; 0 on success.
 */
int
string_from_cstr(const char *cs, String *s);

/*
 * Constructs a String from a stream and a delimiter and
 * stores the result in s.
 * The resulting String does not contain the delimiter.
 *
 * Returns -1 on error; 0 on success.
 */
int
string_from_stream(FILE *stream, int delim, String *s);

/*
 * Loads a whole file into s.
 *
 * Returns -1 on error; 0 on success.
 */
int
string_from_file(const char *filename, String *s);

/*
 * Constructs a String from an StrView and stores the
 * result in s.
 *
 * Returns -1 on error; 0 on success.
 */
int
string_from_sv(StrView sv, String *s);

/*
 * Constructs a String from a formatted string and stores
 * the result in s.
 *
 * Returns -1 on error; 0 on success.
 *
 * For example,
 *
 * ```
 * String s;
 * string_from_fmt(&s, "Hello, %s!", "world");
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
 * Constructs an StrView from s. If s is later freed, the
 * resulting StrView becomes invalidated
 */
StrView
string_to_sv(String s);

/*
 * Copies dst to src.
 * Both strings need to be freed separately.
 *
 * Returns -1 on error; 0 on success.
 */
int
string_copy(String src, String *dst);

/*
 * Moves dst to src.
 * src becomes invalidated, and only dst
 * must be freed.
 */
void
string_move(String *src, String *dst);

/*
 * Concatenates s1 and s2 and stores the result
 * in dst.
 *
 * Returns -1 on error; 0 on success.
 */
int
string_concat(String s1, String s2, String *dst);

/*
 * Frees the memory occupied by s
 */
void
string_free(String *s);

/*
 * Checks if string is empty. This function only
 * checks if the __size of the string is 0, not its
 * __capacity. In other words, if you want to check
 * if s should be freed, you should use string_is_allocated.
 */
bool
string_is_empty(String s);

/*
 * Checks if s occupies memory. Use this function
 * to determine if s should be freed or not.
 */
bool
string_is_allocated(String s);

/*
 * Pushes c to s.
 *
 * Returns -1 on error; 0 on success.
 */
int
string_push(String *s, char c);

/*
 * Pops last char from s and stores it to c.
 *
 * Returns -1 on error; 0 on success.
 */
int
string_pop(String *s, char *c);

/*
 * Appends src to s.
 *
 * Returns -1 on error; 0 on success.
 */
int
string_append(String *s, String src);

#endif /* STRING_H */