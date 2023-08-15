
#ifndef _FC_MEMORY_H
#define _FC_MEMORY_H

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include "string.h"

typedef void (*fc_memory_oom_notify_func)(const size_t curr_size);

extern fc_memory_oom_notify_func g_oom_notify;

#define fc_malloc(size)  fc_malloc_ex(__FILE__, __LINE__, size)
#define fc_realloc(ptr, size)  fc_realloc_ex(__FILE__, __LINE__, ptr, size)
#define fc_calloc(count, size)  fc_calloc_ex(__FILE__, __LINE__, count, size)
#define fc_strdup(str)  fc_strdup_ex(__FILE__, __LINE__, str)

void *fc_malloc_ex(const char *file,const int line, size_t size);
void *fc_realloc_ex(const char *file,const int line, void *ptr, size_t size);
char *fc_strdup_ex(const char *file,const int line, const char *str);
void *fc_calloc_ex(const char *file,const int line, size_t count, size_t size);

#endif
