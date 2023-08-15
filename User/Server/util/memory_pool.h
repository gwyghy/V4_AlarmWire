
#ifndef __MEMORY_POOL_H__
#define __MEMORY_POOL_H__

#include "includes.h"

#ifdef __cplusplus
extern "C" {
#endif
	
struct memp {
  struct memp *next;
};


#define  MEMORYPOOL_ALIGNMENT           4

#ifndef MEMPOOL_ALIGN_SIZE
#define MEMPOOL_ALIGN_SIZE(size) (((size) + MEMORYPOOL_ALIGNMENT - 1) & ~(MEMORYPOOL_ALIGNMENT-1))
#endif

/* No sanity checks
 * We don't need to preserve the struct memp while not allocated, so we
 * can save a little space and set MEMP_SIZE to 0.
 */
#define MEMORYPOOL_SIZE           0
#define MEMORYPOOL_ALIGN_SIZE(x) (MEMPOOL_ALIGN_SIZE(x))

#ifndef  MEMORYPOOL_ALIGN
#define  MEMORYPOOL_ALIGN(addr) ((void *)(((mem_ptr_t)(addr) + MEMORYPOOL_ALIGNMENT - 1) & ~(mem_ptr_t)(MEMORYPOOL_ALIGNMENT-1)))
#endif


/* Create the list of all memory pools managed by memp. MEMP_MAX represents a NULL pool at the end */
typedef enum {
#define MEMORYPOOL(name,num,size,desc)  MEMORYPOOL_##name,
#include "memory_pool_def.h"
  MEMORYPOOL_MAX
} mempool_t;



void  mempool_init(void);
void *mempool_malloc(mempool_t type);
void  mempool_free(mempool_t type, void *mem);


#ifdef __cplusplus
}
#endif

#endif 
