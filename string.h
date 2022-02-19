#ifndef STRING_H
#define STRING_H

#include <stddef.h>
#include "../str_view/str_view.h"

typedef struct String {
	char *__buffer;
	size_t __size;
	size_t __capacity;
} String;

#define STRING_FMT "%.*s"
#define STRING_ARG(s) (int)(s).__size, (s).__buffer

/* Constructors */

/**
 * @brief Returns an empty String
 */
String
string_empty();

/**
 * @brief Creates a String from a null terminated c string.
 * @Errors
 * This function allocates space, so it can fail.
 * To determine if it failed, use string_is_null() on the
 * returned String.
 * @Undefined
 * Undefined behaviour if buf is NULL
 * @Warning buf @b must be null terminated, because the
 * function uses strlen to determine its length.
 */
String
string_from_cstr(const char *buf);

/**
 * @brief Creates a string with the given capacity.
 * @Errors
 * This function allocates space, so it can fail. To determine if it failed,
 * use string_is_null() on the returned String.
 */
String
string_with_capacity(size_t capacity);

/**
 * @brief Creates a String from an StrView.
 * @Errors
 * This function allocates space, so it can fail.
 * To determine if it failed, use string_is_null() on the
 * returned String.
 */
String
string_from_sv(StrView s);

/**
 * @brief Copies s to a new String
 * @Errors
 * This function allocates space, so it can fail. To determine if it failed,
 * use string_is_null() on the returned String.
 */
String
string_copy(String s);

/**
 * @brief Moves s to the returned String. Only the returned String must
 * be freed.
 */
String
string_move(String *s);

/**
 * Constructs an StrView from s.
 */
StrView
string_to_sv(String s);

/* Destructor */

/**
 * @brief Frees the memory allocated for s.
 */
void
string_free(String *s);

/* Queries */

/**
 * @brief Returns the size of s.
 */
size_t
string_size(String s);

/**
 * @brief Returns the capacity of s.
 */
size_t
string_capacity(String s);

/**
 * Checks if s is empty.
 */
bool
string_is_empty(String s);

/**
 * @brief Checks if s is the null string. Note that string_empty() does
 * not return a null string, so @code string_is_null(string_empty()) @endcode
 * will return false.
 */
bool
string_is_null(String s);

/* Accessors */

/**
 * @brief Returns the idx-th char in s.
 * @Undefined
 * Undefined behaviour if idx is less than or equal to the size of s.
 */
char
string_at(String s, size_t idx);

/**
 * @brief Returns a reference to the idx-th char in s
 * @Undefined
 * Undefined behaviour if idx is less than or equal to the size of s.
 */
char *
string_ref(String s, size_t idx);

/* Modifiers */

/**
 * @brief Inserts c into the idx-th position of s.
 * @Errors This function fails if idx is larger or
 * equal to the size of s, or because of a potential
 * allocation failure. To determine if it failed, use
 * string_is_null() on s after the call.
 * @Undefined Undefined behaviour is caused if s is NULL.
 */
void
string_insert(String *s, size_t idx, char c);

/**
 * @brief Inserts a copy of v into the idx-th position of s.
 * @Errors This function fails if idx is larger or equal to the
 * size of s, or because of a potential allocation failure. To
 * determine if it failed, use string_is_null() on s after the call.
 * @Undefined Undefined behaviour is caused if s is NULL.
 */
void
string_insert_sv(String *s, size_t idx, StrView v);

/**
 * @brief Pushes c to the end of s.
 * @Errors This function may allocate space, so it can fail.
 * To determine if it failed, use string_is_null() on s after
 * the call.
 * @Undefined Undefined behaviour is caused if s is NULL.
 */
void
string_push(String *s, char c);

/**
 * @brief Pushes a copy of v to the end of s
 * @Errors This function may allocate space, so it can fail.
 * To determine if it failed, use string_is_null() on s after
 * the call.
 * @Undefined Undefined behaviour is caused if s is NULL.
 */
void
string_push_sv(String *s, StrView v);

/**
 * @brief Pops and returns the last character in s
 * @Undefined Undefined behaviour is caused if s is NULL, or
 * if s is empty.
 */
char
string_pop(String *s);

/**
 * @brief Removes and returns the idx-th character in s.
 * @Undefined Undefined behaviour is caused if s is NULL, if
 * s is empty, or if idx is greater than or equal to the size
 * of s.
 */
char
string_remove(String *s, size_t idx);

#endif /* STRING_H */
