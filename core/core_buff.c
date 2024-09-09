/* Buffer module for JustCoderdev Core library v2
 * */

#define CORE_LOG_MODULE "buffer"

#include <core.h>

#include <stdarg.h>
#include <stdio.h>
extern int vsnprintf(char* str, size_t size, const char *format, va_list ap);

#include <assert.h>
#include <stdlib.h>
#include <unistd.h>

extern n32 buffer_fmt(n32 len, char* buffer, CString format, ...)
{
	int size;

	va_list args;
	va_start(args, format);

	size = vsnprintf(buffer, len, format, args);
	if(size < 0) {
		core_log(CORE_ERROR, "Could not format buffer \"%s\": %s",
				format, strerror(errno));
		exit(failure);
	}

	if((n32)size >= len)
	{
		core_log(CORE_WARN,
				"Truncation of buffer_fmt with format '%s' (size %u >= len %u)\n",
				format, size, len);
	}

	va_end(args);
	return (n32)size;
}

/* Return the index of the chr or -1 */
i64 buffer_find_chr(char chr, char *buffer, n64 len) {
	i64 i;

	assert(len < (n64)2e63);
	for(i = 0; i < (i64)len; ++i)
	{
		if(buffer[i] == chr) return i;
	}

	return -1;
}

/* Return the index of the first char that is not a delimiter or -1 */
i64 buffer_skip_str(char *del, n64 del_len, char *buffer, n64 buff_len) {
	(void)del, (void)del_len, (void)buffer, (void)buff_len;
	assert(0 && "NOT IMPLEMENTED buffer_skip_str");
	return -1;
}

n64 buffer_copy_until_chr(char delimiter,
            char *src_buffer, n64 src_len,
            char *dest_buffer, n64 dest_len)
{
	n64  i, min_len = min(src_len, dest_len);
	char curr;

	for(i = 0; i < min_len - 1; ++i)
	{
		curr = src_buffer[i];
		if(curr == delimiter)
		{
			dest_buffer[i] = '\0';
			return i;
		}

		dest_buffer[i] = curr;
	}

	return 0;
}

n64 buffer_copy_until_str(char *delimiter, n64 del_len,
            char *src_buffer, n64 src_len,
            char *dest_buffer, n64 dest_len)
{
	n64   i, j, min_len = min(src_len, dest_len);
	char *curr;

	for(i = 0; i < min_len - 1; ++i)
	{
		bool match = true;
		curr = src_buffer + i;

		if(src_len - i < del_len) return 0;

		/* strcmp */
		for(j = 0; j < del_len; ++j)
		{
			if(curr[j] != delimiter[j])
			{
				match = false;
				break;
			}
		}

		if(match)
		{
			dest_buffer[i] = '\0';
			return i;
		}

		dest_buffer[i] = *curr;
	}

	return 0;
}

void buffer_put_to_file(FILE *file, n32 buff_len, char *buffer)
{
	n32 i;
	for(i = 0; i < buff_len; ++i)
	{
		switch(buffer[i])
		{
#if 0
			case '\r':
				putc('\\', file);
				putc('r', file);
				break;

			case '\n':
				putc('\\', file);
				putc('n', file);
				putc('\n', file);
				break;
#endif
			/* TODO: Check for errors */
			default: putc(buffer[i], file);
		}
	}
	fflush(file);
}

void buffer_write_tofd_(int fd, n32 buff_len, char *buffer, char* file, int line)
{
	ssize_t sent_bytes = write(fd, buffer, buff_len);
	if(sent_bytes < 0) {
		core_log(CORE_ERROR_, "Cannot write buffer to fd: %s\n",
				strerror(errno));
		exit(failure);
	}

	core_log(CORE_DEBUG_, "Writing %d bytes\n", sent_bytes);
}

bool buffer_equals(n64 buffA_len, n8* buffA, n64 buffB_len, n8* buffB)
{
	n64 i;

	if(buffA_len != buffB_len) return false;

	for(i = 0; i < buffA_len; ++i) {
		if(buffA[i] != buffB[i]) return false;
	}

	return true;
}

#undef CORE_LOG_MODULE
#define CORE_LOG_MODULE NULL
