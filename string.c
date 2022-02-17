#include "string.h"

#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <stdarg.h>

#define INITIAL_CAPACITY 8
#define GROWTH_FACTOR 2

String
string_empty()
{
	return (String){.buffer = NULL, .size = 0, .capacity = 0};
}

int
string_from_cstr(const char *cs, String *s)
{
	String str;

	str.size = str.capacity = strlen(cs);

	if (str.capacity == 0) {
		*s = string_empty();
		return 0;
	}

	if ((str.buffer = malloc(str.capacity * sizeof(char))) == NULL)
		return -1;

	memmove(str.buffer, cs, str.size);

	*s = str;

	return 0;
}

int
string_from_stream(FILE *stream, int delim, String *s)
{
	char *buffer = NULL;
	size_t len = 0;
	ssize_t bytes;

	bytes = getdelim(&buffer, &len, delim, stream);

	if (bytes == -1)
		return -1;

	if (buffer[bytes - 1] == delim)
		bytes--;

	s->buffer = buffer;
	s->size = (size_t)bytes;
	s->capacity = (size_t)bytes;

	return 0;
}

int
string_from_sv(StrView sv, String *s)
{
	String str;

	str.size = str.capacity = sv.__size;
	if ((str.buffer = malloc(str.size * sizeof(char))) == NULL)
		return -1;

	*s = str;

	return 0;
}

int
string_from_fmt(String *s, const char *template, ...)
{
	int res;
	va_list args;

	va_start(args, template);
	res = string_from_vfmt(s, template, args);
	va_end(args);

	return res;
}

int
string_from_vfmt(String *s, const char *template, va_list args)
{
	String str;
	va_list args_cp;
	int bytes;

	va_copy(args_cp, args);

	if ((bytes = vsnprintf(NULL, 0, template, args)) < 0) {
		va_end(args_cp);
		return -1;
	}

	str.buffer = malloc((bytes + 1) * sizeof(char));

	if (vsnprintf(str.buffer, bytes + 1, template, args_cp) < 0) {
		va_end(args_cp);
		free(str.buffer);
		return -1;
	}

	str.size = str.capacity = bytes;
	*s = str;

	va_end(args_cp);
	return 0;
}

StrView
string_to_sv(String s)
{
	return sv_from_parts(s.buffer, s.size);
}

void
string_free(String *s)
{
	free(s->buffer);
	s->buffer = NULL;
	s->size = 0;
	s->capacity = 0;
}

bool
string_is_empty(String s)
{
	return s.size == 0;
}

int
string_push(String *s, char c)
{
	if (s->size == s->capacity) {
		s->capacity = s->capacity == 0 ? INITIAL_CAPACITY
					       : s->capacity * GROWTH_FACTOR;
		if (
		  (s->buffer = realloc(s->buffer, s->capacity * sizeof(char)))
		  == NULL)
			return -1;
	}
	s->buffer[s->size++] = c;
	return 0;
}

int
string_pop(String *s, char *c)
{
	if (s->size == 0)
		return -1;
	*c = s->buffer[--s->size];
	return 0;
}

int
string_append(String *s, String src)
{
	if (s->size + src.size > s->capacity) {
		s->capacity = s->size + src.size;
		if (
		  (s->buffer = realloc(s->buffer, s->capacity * sizeof(char)))
		  == NULL)
			return -1;
	}
	memmove(s->buffer + s->size, src.buffer, src.size);
	s->size += src.size;
	return 0;
}

int
string_copy(String src, String *dst)
{
	String str;

	if ((str.buffer = malloc(src.capacity * sizeof(char))) == NULL)
		return -1;

	memmove(str.buffer, src.buffer, src.size * sizeof(char));
	str.size = src.size;
	str.capacity = src.capacity;

	*dst = str;

	return 0;
}

void
string_move(String *src, String *dst)
{
	dst->buffer = src->buffer;
	dst->size = src->size;
	dst->capacity = src->capacity;

	*src = string_empty();
}

int
string_concat(String s1, String s2, String *dst)
{
	size_t cap;
	String s;

	s.size = s.capacity = s1.size + s2.size;
	if ((s.buffer = malloc(s.capacity * sizeof(char))) == NULL)
		return -1;

	*dst = s;
}