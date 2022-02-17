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
	return (String){.__buffer = NULL, .__size = 0, .__capacity = 0};
}

int
string_from_cstr(const char *cs, String *s)
{
	String str;

	str.__size = str.__capacity = strlen(cs);

	if (str.__capacity == 0) {
		*s = string_empty();
		return 0;
	}

	if ((str.__buffer = malloc(str.__capacity * sizeof(char))) == NULL)
		return -1;

	memmove(str.__buffer, cs, str.__size);

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

	s->__buffer = buffer;
	s->__size = (size_t)bytes;
	s->__capacity = (size_t)bytes;

	return 0;
}

int
string_from_file(const char *filename, String *s)
{
	String str;
	FILE *file;

	file = fopen(filename, "r");

	if (file == NULL)
		return -1;

	if (string_from_stream(file, '\0', &str) == -1) {
		fclose(file);
		return -1;
	}

	fclose(file);

	*s = str;

	return 0;
}

int
string_from_sv(StrView sv, String *s)
{
	String str;

	str.__size = str.__capacity = sv.__size;
	if ((str.__buffer = malloc(str.__size * sizeof(char))) == NULL)
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

	str.__buffer = malloc((bytes + 1) * sizeof(char));

	if (vsnprintf(str.__buffer, bytes + 1, template, args_cp) < 0) {
		va_end(args_cp);
		free(str.__buffer);
		return -1;
	}

	str.__size = str.__capacity = bytes;
	*s = str;

	va_end(args_cp);
	return 0;
}

StrView
string_to_sv(String s)
{
	return sv_from_parts(s.__buffer, s.__size);
}

char
string_at(String s, size_t idx)
{
	return s.__buffer[idx];
}

int
string_at_s(String s, size_t idx, char *c)
{
	return sv_at_s(string_to_sv(s), idx, c);
}

const char *
string_at_ref(String s, size_t idx)
{
	return &s.__buffer[idx];
}

int
string_at_ref_s(String s, size_t idx, const char **c)
{
	return sv_at_ref_s(string_to_sv(s), idx, c);
}

void
string_free(String *s)
{
	free(s->__buffer);
	s->__buffer = NULL;
	s->__size = 0;
	s->__capacity = 0;
}

bool
string_is_empty(String s)
{
	return s.__size == 0;
}

bool
string_is_allocated(String s)
{
	return s.__buffer != NULL;
}

int
string_push(String *s, char c)
{
	if (s->__size == s->__capacity) {
		s->__capacity = s->__capacity == 0
				? INITIAL_CAPACITY
				: s->__capacity * GROWTH_FACTOR;
		if (
		  (s->__buffer =
		     realloc(s->__buffer, s->__capacity * sizeof(char)))
		  == NULL)
			return -1;
	}
	s->__buffer[s->__size++] = c;
	return 0;
}

int
string_pop(String *s, char *c)
{
	if (s->__size == 0)
		return -1;
	*c = s->__buffer[--s->__size];
	return 0;
}

int
string_append(String *s, String src)
{
	if (s->__size + src.__size > s->__capacity) {
		s->__capacity = s->__size + src.__size;
		if (
		  (s->__buffer =
		     realloc(s->__buffer, s->__capacity * sizeof(char)))
		  == NULL)
			return -1;
	}
	memmove(s->__buffer + s->__size, src.__buffer, src.__size);
	s->__size += src.__size;
	return 0;
}

int
string_copy(String src, String *dst)
{
	String str;

	if ((str.__buffer = malloc(src.__size * sizeof(char))) == NULL)
		return -1;

	memmove(str.__buffer, src.__buffer, src.__size * sizeof(char));
	str.__size = src.__size;
	str.__capacity = src.__size;

	*dst = str;

	return 0;
}

void
string_move(String *src, String *dst)
{
	dst->__buffer = src->__buffer;
	dst->__size = src->__size;
	dst->__capacity = src->__capacity;

	*src = string_empty();
}

int
string_concat(String s1, String s2, String *dst)
{
	size_t cap;
	String s;

	s.__size = s.__capacity = s1.__size + s2.__size;
	if ((s.__buffer = malloc(s.__capacity * sizeof(char))) == NULL)
		return -1;

	*dst = s;
}