/* Stuff module for JustCoderdev Core library v2
 * */

#include <core.h>

#include <assert.h>
#include <stdlib.h>

char* shift(int* argc, char*** argv)
{
	char* t;
	assert(*argc > 0);

	(*argc)--;
	t = (*argv)[0];
	(*argv)++;

	return t;
}

/* return -1 on error */
long file_size_get(FILE* file, CString file_name)
{
	long file_size = -1,
	     file_start_pos = ftell(file);

	if(file_start_pos == -1) {
		core_log(CORE_ERROR, "Cannot save cursor position of file '%s': %s\n",
				file_name, strerror(errno));
		exit(failure);
	}

	if(fseek(file, 0L, SEEK_END) == -1) {
		core_log(CORE_ERROR, "Cannot seek end of file '%s': %s\n",
				file_name, strerror(errno));
		exit(failure);
	}

	file_size = ftell(file);
	if(file_size == -1) {
		core_log(CORE_ERROR, "Cannot get size of file '%s': %s\n",
				file_name, strerror(errno));
		exit(failure);
	}

	if(fseek(file, file_start_pos, SEEK_SET) == -1) {
		core_log(CORE_ERROR, "Cannot reset cursor of file '%s': %s\n",
				file_name, strerror(errno));
		exit(failure);
	}

	return file_size;
}
