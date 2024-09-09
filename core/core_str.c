/* String module for JustCoderdev Core library v5
 * */

#define CORE_LOG_MODULE "string"

#include <core.h>

#include <stdarg.h>
#include <stdio.h>
extern int vsnprintf(char* str, size_t size, const char *format, va_list ap);

#include <stdlib.h>
#include <assert.h>
#include <errno.h>
#include <string.h>


/* #define DEBUG_STRING_ENABLE 1 */

void fstring_new_(FString* string, n32 capacity, char* file, int line)
{
	string->chars = malloc_(capacity, file, line);
	if(string->chars == NULL) {
		core_log(CORE_ERROR_, "Could not mallocate fstring, error: %s",
				strerror(errno));
		exit(failure);
	}

#if DEBUG_STRING_ENABLE
	core_log(CORE_DEBUG_, "Mallocated string at %p with size %lu\n",
			string->chars, capacity);
#endif

	string->count = 0;
}

void dstring_new_(DString* string, n32 capacity, char* file, int line)
{
	fstring_new_(&string->data, capacity, file, line);
	string->capacity = capacity;
}


void fstring_new_from_(FString* string, n32 text_len, char* text, char* file, int line)
{
	string->chars = malloc_(text_len, file, line);
	if(string->chars == NULL) {
		core_log(CORE_ERROR_, "Could not mallocate fstring, error: %s",
				strerror(errno));
		exit(failure);
	}

#if DEBUG_STRING_ENABLE
	core_log(CORE_DEBUG_, "Mallocated string at %p with size %lu\n",
			string->chars, text_len);
#endif

	strncpy(string->chars, text, text_len);
	string->count = text_len;
}

void dstring_new_from_(DString* string, n32 text_len, char* text, char* file, int line)
{
	fstring_new_from_(&string->data, text_len, text, file, line);
	string->capacity = text_len;
}

void fstring_from_(FString* string, n32 text_len, char* text, char* file, int line)
{
	assert(string->chars != NULL);

	if(string->count < text_len)
	{
		core_log(CORE_WARN_, "Could not fit text inside fstring (%u > %u)\n",
				text_len, string->count);
		/* return failure; */
	}

	strncpy(string->chars, text, min(text_len, string->count));
}

void dstring_from_(DString* string, n32 text_len, char* text, char* file, int line)
{
	assert(string->data.chars != NULL);

	if(string->capacity < text_len)
	{
		string->data.chars = realloc_(string->data.chars, text_len, file, line);
		if(string->data.chars == NULL) {
			core_log(CORE_ERROR_, "Could not reallocate string: %s",
					strerror(errno));
			exit(failure);
		}

		string->capacity = text_len;
	}

	strncpy(string->data.chars, text, text_len);
	string->data.count = text_len;
}

void dstring_nterm_(DString* string, char* file, int line)
{
	if(string->data.chars == NULL || string->data.count == 0
			|| string->data.chars[string->data.count - 1] != '\0')
	{
#if DEBUG_STRING_ENABLE
		core_log(CORE_DEBUG_, "Null terminating string '"STR_FMT"' at %p...\n",
				DSTR(*string), string->chars);
#endif

		dstring_append_(string, '\0', file, line);
	}
}

CString dstring_as_CStr_(DString* string, char* file, int line) {
	dstring_nterm_(string, file, line);
	return string->data.chars;
}

CString fstring_as_CStr_(FString* string, char* file, int line)
{
	bool isCString = true;
	CString error_message = NULL;

	if(string->chars == NULL) {
		isCString = false;
		error_message = "String is NULL";
		goto fstring_as_CStr_check;
	}

	if(string->count == 0) {
		isCString = false;
		error_message = "String has length 0";
		goto fstring_as_CStr_check;
	}

	if(string->chars[string->count - 1] != '\0') {
		isCString = false;
		error_message = "String is not \\0 terminated";
		goto fstring_as_CStr_check;
	}

fstring_as_CStr_check:
	if(!isCString)
	{
		core_log(CORE_ERROR_,
				"Check for converting fstring to CString failed: %s\n",
				error_message);
		exit(failure);
	}

	return string->chars;
}

void dstring_clear(DString* string) {
	string->data.count = 0;
}

void dstring_fmt(DString* string, CString format, ...) {
	int size;

	va_list args;
	va_start(args, format);

	size = vsnprintf(string->data.chars, string->capacity, format, args);
	if(size < 0) {
		core_log(CORE_ERROR, "Could not format string '%s': %s\n",
				format, strerror(errno));
		exit(failure);
	}

	if((n32)size >= string->capacity)
	{
		core_log(CORE_WARN, "Truncation of string_fmt with format '%s'\n", format);
		string->data.count = string->capacity - 1;
	} else {
		/* vsnprintf always null terminates string */
		string->data.count = (n32)size + 1;
	}

	va_end(args);
}

void dstring_remove(DString* string, n32 count) {
	string->data.count -= pclamp(count, string->data.count);
}

void dstring_append_(DString* string, char chr, char* file, int line)
{
	if(string->data.chars == NULL)
		dstring_new_(string, 8, file, line);

	if(string->capacity < string->data.count + 1)
	{
		n32 new_size = (string->data.count + 1) * 2;
		assert(new_size > string->data.count && "Failed overflow check");

		string->data.chars = realloc_(string->data.chars, new_size, file, line);
		if(string->data.chars == NULL) {
			core_log(CORE_ERROR_, "Could not resize string: %s\n",
					strerror(errno));
			exit(failure);
		}

		string->capacity = new_size;
	}

	string->data.chars[string->data.count] = chr;
	string->data.count++;
}


void fstring_free_(FString* string, char* file, int line)
{
	if(string->chars != NULL) {

#if DEBUG_STRING_ENABLE
		core_log(CORE_DEBUG_, "Freeing string at %p ('"STR_FMT"')\n",
				string->chars, FSTR(*string));
#endif

		free_(string->chars, file, line);
	}

	string->count = 0;
}

void dstring_free_(DString* string, char* file, int line) {
	fstring_free_(&string->data, file, line);
	string->capacity = 0;
}

bool fstring_equals(FString strA, n32 strB_len, CString strB) {
	n32 i;

	if(strA.count != strB_len) return false;

	for(i = 0; i < strB_len; ++i)
		if(strA.chars[i] != strB[i])
			return false;

	return true;
}

bool fstring_equals_CStr(FString strA, CString strB) {
	assert(strlen(strB) < (n32)-1);
	return fstring_equals(strA, CSTR(strB));
}

#undef CORE_LOG_MODULE
#define CORE_LOG_MODULE NULL
