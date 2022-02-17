#ifndef STRING_H
#define STRING_H

#include <stdio.h>
#include "str_view/str_view.h"

typedef struct String {
	char *__buffer;
	size_t __size;
	size_t __capacity;
} String;

#define STRING_FMT "%.*s"
#define STRING_ARG(s) (int)(s).__size, (s).__buffer

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
 * Returns the character in the idx-th position.
 * This function does *not* bounds-check.
 * For a safe alternative, use string_at_s.
 */
char
string_at(String s, size_t idx);

/*
 * If idx is less than the string's size, returns
 * 0 and assigns the idx-th character to c;
 * else returns -1.
 */
int
string_at_s(String s, size_t idx, char *c);

/*
 * Returns a reference to the character in the
 * idx-th position. This function does *not* bounds-check.
 * For a safe alternative, use string_at_ref_s.
 */
const char *
string_at_ref(String s, size_t idx);

/*
 * If idx is less than the string's size, returns
 * 0 and assigns the reference to the idx-th character
 * to c; else returns -1.
 */
int
string_at_ref_s(String s, size_t idx, const char **c);

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
 * strcmp for String
 */
int
string_cmp(String s1, String s2);

/*
 * Splits the string into the part before the delimiter and
 * the part after the delimiter, and assigns them to pre
 * and post respectively.
 *
 * If the delimiter is found in the string, the function returns
 * true, else it returns false.
 *
 * For example:
 *
 * `string_split(string_from_cstr("Hello!World", '!', &pre, &post);`
 *
 * will return true, and will assign "Hello" to pre, and "World" to post,
 * whereas
 *
 * `string_split(string_from_cstr("Hello World", '!', &prem &post);`
 *
 * will return false, and will assign "Hello World" to pre, and
 * post will get assigned an empty string starting at the character
 * after the end of s.
 *
 * Any or both of the pre and post args can be null.
 */
bool
string_split(String s, char delim, StrView *pre, StrView *post);

/*
 * Same as string_split, but only checks the first n characters for the
 * delimiter
 */
bool
string_split_n(String s, char delim, size_t n, StrView *pre, StrView *post);

/*
 * Checks if s contains the character c
 */
bool
string_contains(String s, char c);

/*
 * Returns true if s contains the character c, and assigns its first occurrence
 * index to idx. Else, it returns false.
 *
 * idx can be null
 */
bool
string_first_occ(String s, char c, size_t *idx);

/*
 * Returns true if s contains the character c, and assigns its last occurrence
 * index to idx. Else, it returns false.
 *
 * idx can be null
 */
bool
string_last_occ(String s, char c, size_t *idx);

/*
 * Returns true if prefix is a prefix of s, else returns false
 */
bool
string_starts_with(String s, StrView prefix);

/*
 * Returns true if postfix is a postfix of s, else returns false
 */
bool
string_ends_with(String s, StrView postfix);

/*
 * Returns the number of occurrences of c in s
 */
size_t
string_count(String s, char c);

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
 * checks if the size of the string is 0, not its
 * capacity. In other words, if you want to check
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