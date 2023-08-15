
#include "fc_memory.h"

fc_memory_oom_notify_func g_oom_notify = NULL;

void *fc_malloc_ex(const char *file,const int line, size_t size)
{
	void *ptr;
	ptr = malloc(size);
	if (ptr == NULL) {
		if (g_oom_notify != NULL) {
			g_oom_notify(size);
		}
	}

	return ptr;
}

void *fc_realloc_ex(const char *file,const int line, void *ptr, size_t size)
{
	void *new_ptr;
	new_ptr = realloc(ptr, size);
	if (new_ptr == NULL) {
		if (g_oom_notify != NULL) {
			g_oom_notify(size);
		}
	}

	return new_ptr;
}

char *fc_strdup_ex(const char *file,const int line, const char *str)
{
	char *output;
	int len;

	len = strlen(str);
	output = (char *)fc_malloc_ex(file, line, len + 1);
	if (output == NULL) {
		return NULL;
	}

	if (len > 0) {
		memcpy(output, str, len);
	}
	*(output + len) = '\0';
	return output;
}

void *fc_calloc_ex(const char *file,const int line, size_t count, size_t size)
{
	void *ptr;
	ptr = calloc(count, size);
	if (ptr == NULL) {
		if (g_oom_notify != NULL) {
			g_oom_notify(count * size);
		}
	}

	return ptr;
}
