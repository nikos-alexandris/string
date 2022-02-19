#include "string.h"

#include <malloc.h>
#include <string.h>
#include <stdint.h>

#define INITIAL_CAPACITY 8
#define GROWTH_FACTOR 2

static void
string_grow(String *s);

static void
string_grow_to(String *s, size_t cap);

static String
string_null();

String
string_empty()
{
	String str;

	str.__size = 0;
	str.__capacity = 0;
	str.__buffer = NULL;

	return str;
}

String
string_with_capacity(size_t capacity)
{
	String str;

	str.__size = 0;
	str.__capacity = capacity;
	str.__buffer = malloc(str.__capacity * sizeof(char));

	if (str.__buffer == NULL)
		return string_null();

	return str;
}

String
string_from_sv(StrView s)
{
	String str;

	str.__size = str.__capacity = s.__size;
	str.__buffer = malloc(str.__capacity * sizeof(char));

	if (str.__buffer == NULL)
		return string_null();

	memmove(str.__buffer, s.__buffer, str.__size);

	return str;
}

String
string_from_cstr(const char *s)
{
	String str;

	str.__size = str.__capacity = strlen(s);
	str.__buffer = malloc(str.__capacity * sizeof(char));

	if (str.__buffer == NULL)
		return string_null();

	memmove(str.__buffer, s, str.__size);

	return str;
}

char
string_at(String s, size_t idx)
{
	return s.__buffer[idx];
}

char *
string_ref(String s, size_t idx)
{
	return &s.__buffer[idx];
}

String
string_copy(String s)
{
	String str;

	str.__size = str.__capacity = s.__size;
	str.__buffer = malloc(str.__capacity * sizeof(char));

	if (str.__buffer == NULL)
		return string_null();

	memmove(str.__buffer, s.__buffer, str.__size);

	return str;
}

String
string_move(String *s)
{
	String str;

	str = *s;
	*s = string_empty();

	return str;
}

StrView
string_to_sv(String s)
{
	return sv_from_parts(s.__buffer, s.__size);
}

void
string_free(String *s)
{
	free(s->__buffer);
	*s = string_empty();
}

bool
string_is_empty(String s)
{
	return s.__buffer == NULL && s.__size == 0;
}

bool
string_is_null(String s)
{
	return s.__buffer == NULL && s.__size == SIZE_MAX && s.__capacity == SIZE_MAX;
}

int
string_cmp(String l, String r)
{
	return strncmp(l.__buffer, r.__buffer, l.__size < r.__size ? l.__size : r.__size);
}

bool
string_eq(String s1, String s2)
{
	return s1.__size == s2.__size
	    && strncmp(s1.__buffer, s2.__buffer, s1.__size) == 0;
}

void
string_insert(String *s, size_t idx, char c)
{
	if (idx >= s->__size) {
		*s = string_null();
		return;
	}

	if (s->__size == s->__capacity) {
		string_grow(s);
		if (string_is_null(*s))
			return;
	}

	memmove(&s->__buffer[idx + 1], &s->__buffer[idx], s->__size - idx);
	s->__buffer[idx] = c;
	s->__size++;
}

void
string_insert_sv(String *s, size_t idx, StrView v)
{
	if (idx >= s->__size) {
		*s = string_null();
		return;
	}

	if (s->__size + v.__size > s->__capacity) {
		string_grow_to(s, s->__size + v.__size);
		if (string_is_null(*s))
			return;
	}

	memmove(&s->__buffer[idx + v.__size], &s->__buffer[idx], s->__size - idx);
	memmove(&s->__buffer[idx], v.__buffer, v.__size);

	s->__size += v.__size;
}

void
string_push(String *s, char c)
{
	if (s->__size == s->__capacity) {
		string_grow(s);
		if (string_is_null(*s))
			return;
	}

	s->__buffer[s->__size++] = c;
}

void
string_push_sv(String *s, StrView v)
{
	if (s->__size + v.__size > s->__capacity) {
		string_grow_to(s, s->__size + v.__size);
		if (string_is_null(*s))
			return;
	}

	memmove(&s->__buffer[s->__size], v.__buffer, v.__size);
	s->__size += v.__size;
}

char
string_pop(String *s)
{
	return s->__buffer[--s->__size];
}

char
string_remove(String *s, size_t idx)
{
	char c;

	c = s->__buffer[idx];

	memmove(&s->__buffer[idx], &s->__buffer[idx + 1], s->__size - idx - 1);
	s->__size--;

	return c;
}

static void
string_grow(String *s)
{
	if (s->__capacity == 0)
		s->__capacity = INITIAL_CAPACITY;
	else
		s->__capacity *= GROWTH_FACTOR;

	s->__buffer = realloc(s->__buffer, s->__capacity * sizeof(char));

	if (s->__buffer == NULL)
		*s = string_null();
}

static void
string_grow_to(String *s, size_t cap)
{
	s->__capacity = cap;
	s->__buffer = realloc(s->__buffer, s->__capacity * sizeof(char));

	if (s->__buffer == NULL)
		*s = string_null();
}

static String
string_null()
{
	String s;

	s.__size = s.__capacity = SIZE_MAX;
	s.__buffer = NULL;

	return s;
}