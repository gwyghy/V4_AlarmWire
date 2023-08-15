
#include "lwip/sys.h"
#include "memory_pool.h"
#include <string.h>


/** This array holds the first free element of each pool.
 *  Elements form a linked list. */
static struct memp *mempool_tab[MEMORYPOOL_MAX];


static const u16_t mempool_sizes[MEMORYPOOL_MAX] = {
#define  MEMORYPOOL(name,num,size,desc)   MEMORYPOOL_ALIGN_SIZE(size),
#include "memory_pool_def.h"
};

/** This array holds the number of elements in each pool. */
static const u16_t mempool_num[MEMORYPOOL_MAX] = {
#define  MEMORYPOOL(name,num,size,desc)  (num),
#include "memory_pool_def.h"
};


/** This is the actual memory used by the pools (all pools in one big block). */
static u8_t mempool_memory[MEMORYPOOL_ALIGNMENT - 1 
#define  MEMORYPOOL(name,num,size,desc) + ( (num) * (MEMORYPOOL_SIZE + MEMORYPOOL_ALIGN_SIZE(size) ) )
#include "memory_pool_def.h"
];


/**
 * Initialize this module.
 * 
 * Carves out memp_memory into linked lists for each pool-type.
 */
void
mempool_init(void)
{
  struct memp *memp;
  u16_t i, j;

  memp = (struct memp *) MEMORYPOOL_ALIGN(mempool_memory);

  /* for every pool: */
  for (i = 0; i < MEMORYPOOL_MAX; ++i) {
    mempool_tab[i] = NULL;
    /* create a linked list of memp elements */
    for (j = 0; j < mempool_num[i]; ++j) {
      memp->next = mempool_tab[i];
      mempool_tab[i] = memp;
      memp = (struct memp *)(void *)((u8_t *)memp + MEMORYPOOL_SIZE + mempool_sizes[i]);
    }
  }

}

/**
 * Get an element from a specific pool.
 *
 * @param type the pool to get an element from
 *
 * the debug version has two more parameters:
 * @param file file name calling this function
 * @param line number of line where this function is called
 *
 * @return a pointer to the allocated memory or a NULL pointer on error
 */
void *
mempool_malloc(mempool_t type)
{
  struct memp *memp;
  SYS_ARCH_DECL_PROTECT(old_level);

  SYS_ARCH_PROTECT(old_level);

  memp = mempool_tab[type];
  
  if (memp != NULL) {
    mempool_tab[type] = memp->next;
    memp = (struct memp*)(void *)((u8_t*)memp + MEMORYPOOL_SIZE);
  } 

  SYS_ARCH_UNPROTECT(old_level);

  return memp;
}

/**
 * Put an element back into its pool.
 *
 * @param type the pool where to put mem
 * @param mem the memp element to free
 */
void
mempool_free(mempool_t type, void *mem)
{
  struct memp *memp;
  SYS_ARCH_DECL_PROTECT(old_level);

  if (mem == NULL) {
    return;
  }
                        
  memp = (struct memp *)(void *)((u8_t*)mem - MEMORYPOOL_SIZE);

  SYS_ARCH_PROTECT(old_level);
  
  memp->next = mempool_tab[type]; 
  mempool_tab[type] = memp;

  SYS_ARCH_UNPROTECT(old_level);
}

