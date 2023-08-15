
#ifndef __MEMORY_H__
#define __MEMORY_H__


#ifdef __cplusplus
extern "C" {
#endif

#include "includes.h"
	

/** All allocated blocks will be MIN_SIZE bytes big, at least!
 * MIN_SIZE can be overridden to suit your needs. Smaller values save space,
 * larger values could prevent too small blocks to fragment the RAM too much. */

#define MEMORY_MAX_SIZE                (40*1024)   //todo 40k ok
#define MEMORY_MIN_SIZE                 32

#if MEMORY_MAX_SIZE > 64000L
	typedef u32_t mem_size_t;
	#define MEM_SIZE_F U32_F
#else
	typedef u16_t mem_size_t;
	#define MEM_SIZE_F U16_F
#endif /* MEM_SIZE > 64000 */

	
#define  MEMORY_ALIGNMENT           4
/** Calculate memory size for an aligned buffer - returns the next highest
 * multiple of MEM_ALIGNMENT (e.g.  MEM_ALIGN_SIZE(3) and
 *  MEM_ALIGN_SIZE(4) will both yield 4 for MEM_ALIGNMENT == 4).
 */
#ifndef  MEMORY_ALIGN_SIZE
#define  MEMORY_ALIGN_SIZE(size) (((size) + MEMORY_ALIGNMENT - 1) & ~(MEMORY_ALIGNMENT-1))
#endif

/** Calculate safe memory size for an aligned buffer when using an unaligned
 * type as storage. This includes a safety-margin on (MEMORY_ALIGNMENT - 1) at the
 * start (e.g. if buffer is u8_t[] and actual data will be u32_t*)
 */
#ifndef  MEMORY_ALIGN_BUFFER
#define  MEMORY_ALIGN_BUFFER(size) (((size) + MEMORY_ALIGNMENT - 1))
#endif

/** Align a memory pointer to the alignment defined by MEMORY_ALIGNMENT
 * so that ADDR % MEMORY_ALIGNMENT == 0
 */
#ifndef  MEMORY_ALIGN
#define  MEMORY_ALIGN(addr) ((void *)(((mem_ptr_t)(addr) + MEMORY_ALIGNMENT - 1) & ~(mem_ptr_t)(MEMORY_ALIGNMENT-1)))
#endif


/**
 * The heap is made up as a list of structs of this type.
 * This does not have to be aligned since for getting its size,
 * we only use the macro SIZEOF_STRUCT_MEM, which automatically alignes.
 */
struct mem {
  /** index (-> ram[next]) of the next struct */
  mem_size_t next;
  /** index (-> ram[prev]) of the previous struct */
  mem_size_t prev;
  /** 1: this area is used; 0: this area is unused */
  u8_t used;
};

/* some alignment macros: we define them here for better source code layout */
#define MEMORY_MIN_SIZE_ALIGNED      MEMORY_ALIGN_SIZE(MEMORY_MIN_SIZE)
#define SIZEOF_STRUCT_MEM     MEMORY_ALIGN_SIZE(sizeof(struct mem))
#define MEMORY_SIZE_ALIGNED   MEMORY_ALIGN_SIZE(MEMORY_MAX_SIZE)

/* Protect the heap only by using a semaphore */
#define  memory_free_DECL_PROTECT()  SYS_ARCH_DECL_PROTECT(lev_free)
#define  memory_free_PROTECT()       SYS_ARCH_PROTECT(lev_free)           //sys_mutex_lock(&mem_mutex)
#define  memory_free_UNPROTECT()     SYS_ARCH_UNPROTECT(lev_free)         //sys_mutex_unlock(&mem_mutex)

/* MEMORY_malloc is protected using semaphore AND  MEM_ALLOC_PROTECT */
#define  MEM_ALLOC_DECL_PROTECT()  SYS_ARCH_DECL_PROTECT(lev_alloc)
#define  MEM_ALLOC_PROTECT()       SYS_ARCH_PROTECT(lev_alloc)
#define  MEM_ALLOC_UNPROTECT()     SYS_ARCH_UNPROTECT(lev_alloc)


void  memory_init(void);
void *memory_trim(void *mem, mem_size_t size);
void *memory_malloc(mem_size_t size);
void  memory_free(void *mem);


#ifdef __cplusplus
}
#endif

#endif /* __MEMORY_H__ */
