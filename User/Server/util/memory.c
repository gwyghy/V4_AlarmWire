
#include "memory.h"
#include <string.h>
#include "lwip/sys.h"



#ifndef  RAM_HEAP_POINTER
	/** the heap. we need one struct mem at the end and some room for alignment */
	u8_t memory_heap[MEMORY_SIZE_ALIGNED + (2*SIZEOF_STRUCT_MEM) + MEMORY_ALIGNMENT]   __attribute__((at(MEM_CCM_UTIL_MEMORY_POS)));;
	#define  RAM_HEAP_POINTER memory_heap
#endif

/** pointer to the heap (ram_heap): for alignment, ram is now a pointer instead of an array */
static u8_t *ram;
/** the last entry, always unused! */
static struct mem *ram_end;
/** pointer to the lowest free block, this is used for faster search */
static struct mem *lfree;

//mem_size_t ptr_test;

/**
 * "Plug holes" by combining adjacent empty struct mems.
 * After this function is through, there should not exist
 * one empty struct mem pointing to another empty struct mem.
 *
 * @param mem this points to a struct mem which just has been freed
 * @internal this function is only called by memory_free() and memory_trim()
 *
 * This assumes access to the heap is protected by the calling function
 * already.
 */
static void
plug_holes(struct mem *mem)
{
  struct mem *nmem;
  struct mem *pmem;

  nmem = (struct mem *)(void *)&ram[mem->next];
  if (mem != nmem && nmem->used == 0 && (u8_t *)nmem != (u8_t *)ram_end) {
    /* if mem->next is unused and not end of ram, combine mem and mem->next */
    if (lfree == nmem) {
      lfree = mem;
    }
    mem->next = nmem->next;
    ((struct mem *)(void *)&ram[nmem->next])->prev = (mem_size_t)((u8_t *)mem - ram);
  }

  /* plug hole backward */
  pmem = (struct mem *)(void *)&ram[mem->prev];
  if (pmem != mem && pmem->used == 0) {
    /* if mem->prev is unused, combine mem and mem->prev */
    if (lfree == mem) {
      lfree = pmem;
    }
    pmem->next = mem->next;
    ((struct mem *)(void *)&ram[mem->next])->prev = (mem_size_t)((u8_t *)pmem - ram);
  }
}

/**
 * Zero the heap and initialize start, end and lowest-free
 */
void
memory_init(void)
{
  struct mem *mem;

  /* align the heap */
  ram = (u8_t *) MEMORY_ALIGN( RAM_HEAP_POINTER);
  /* initialize the start of the heap */
  mem = (struct mem *)(void *)ram;
  mem->next = MEMORY_SIZE_ALIGNED;
  mem->prev = 0;
  mem->used = 0;
  /* initialize the end of the heap */
  ram_end = (struct mem *)(void *)&ram[MEMORY_SIZE_ALIGNED];
  ram_end->used = 1;
  ram_end->next = MEMORY_SIZE_ALIGNED;
  ram_end->prev = MEMORY_SIZE_ALIGNED;

  /* initialize the lowest-free pointer to the start of the heap */
  lfree = (struct mem *)(void *)ram;
}

/**
 * Put a struct mem back on the heap
 *
 * @param rmem is the data portion of a struct mem as returned by a previous
 *             call to memory_malloc()
 */
void
memory_free(void *rmem)
{
  struct mem *mem;
   memory_free_DECL_PROTECT();

  if (rmem == NULL) {
    return;
  }

  if ((u8_t *)rmem < (u8_t *)ram || (u8_t *)rmem >= (u8_t *)ram_end) {
    SYS_ARCH_DECL_PROTECT(lev);
    return;
  }
  /* protect the heap from concurrent access */
   memory_free_PROTECT();
  /* Get the corresponding struct mem ... */
  mem = (struct mem *)(void *)((u8_t *)rmem - SIZEOF_STRUCT_MEM);
  /* ... and is now unused. */
  mem->used = 0;

  if (mem < lfree) {
    /* the newly freed struct is now the lowest */
    lfree = mem;
  }

  /* finally, see if prev or next are free also */
  plug_holes(mem);
//  ptr_test = (mem_size_t)((u8_t *)lfree - ram);

   memory_free_UNPROTECT();
}

//u16_t getTestMemValue(void)
//{
//	ptr_test = (mem_size_t)((u8_t *)lfree - ram);
//	return ptr_test;
//}

/**
 * Shrink memory returned by memory_malloc().
 *
 * @param rmem pointer to memory allocated by memory_malloc the is to be shrinked
 * @param newsize required size after shrinking (needs to be smaller than or
 *                equal to the previous size)
 * @return for compatibility reasons: is always == rmem, at the moment
 *         or NULL if newsize is > old size, in which case rmem is NOT touched
 *         or freed!
 */
void *
memory_trim(void *rmem, mem_size_t newsize)
{
  mem_size_t size;
  mem_size_t ptr, ptr2;
  struct mem *mem, *mem2;
  /* use the FREE_PROTECT here: it protects with sem OR SYS_ARCH_PROTECT */
   memory_free_DECL_PROTECT();

  /* Expand the size of the allocated memory region so that we can
     adjust for alignment. */
  newsize =  MEMORY_ALIGN_SIZE(newsize);

  if(newsize < MEMORY_MIN_SIZE_ALIGNED) {
    /* every data block must be at least MEMORY_MIN_SIZE_ALIGNED long */
    newsize = MEMORY_MIN_SIZE_ALIGNED;
  }

  if (newsize > MEMORY_SIZE_ALIGNED) {
    return NULL;
  }

  if ((u8_t *)rmem < (u8_t *)ram || (u8_t *)rmem >= (u8_t *)ram_end) {
    SYS_ARCH_DECL_PROTECT(lev);
    return rmem;
  }
  /* Get the corresponding struct mem ... */
  mem = (struct mem *)(void *)((u8_t *)rmem - SIZEOF_STRUCT_MEM);
  /* ... and its offset pointer */
  ptr = (mem_size_t)((u8_t *)mem - ram);

  size = mem->next - ptr - SIZEOF_STRUCT_MEM;

  if (newsize > size) {
    /* not supported */
    return NULL;
  }
  if (newsize == size) {
    /* No change in size, simply return */
    return rmem;
  }

  /* protect the heap from concurrent access */
   memory_free_PROTECT();

  mem2 = (struct mem *)(void *)&ram[mem->next];
  if(mem2->used == 0) {
    /* The next struct is unused, we can simply move it at little */
    mem_size_t next;
    /* remember the old next pointer */
    next = mem2->next;
    /* create new struct mem which is moved directly after the shrinked mem */
    ptr2 = ptr + SIZEOF_STRUCT_MEM + newsize;
    if (lfree == mem2) {
      lfree = (struct mem *)(void *)&ram[ptr2];
    }
    mem2 = (struct mem *)(void *)&ram[ptr2];
    mem2->used = 0;
    /* restore the next pointer */
    mem2->next = next;
    /* link it back to mem */
    mem2->prev = ptr;
    /* link mem to it */
    mem->next = ptr2;
    /* last thing to restore linked list: as we have moved mem2,
     * let 'mem2->next->prev' point to mem2 again. but only if mem2->next is not
     * the end of the heap */
    if (mem2->next != MEMORY_SIZE_ALIGNED) {
      ((struct mem *)(void *)&ram[mem2->next])->prev = ptr2;
    }
    /* no need to plug holes, we've already done that */
  } else if (newsize + SIZEOF_STRUCT_MEM + MEMORY_MIN_SIZE_ALIGNED <= size) {
    /* Next struct is used but there's room for another struct mem with
     * at least MEMORY_MIN_SIZE_ALIGNED of data.
     * Old size ('size') must be big enough to contain at least 'newsize' plus a struct mem
     * ('SIZEOF_STRUCT_MEM') with some data ('MEMORY_MIN_SIZE_ALIGNED').
     * @todo we could leave out MEMORY_MIN_SIZE_ALIGNED. We would create an empty
     *       region that couldn't hold data, but when mem->next gets freed,
     *       the 2 regions would be combined, resulting in more free memory */
    ptr2 = ptr + SIZEOF_STRUCT_MEM + newsize;
    mem2 = (struct mem *)(void *)&ram[ptr2];
    if (mem2 < lfree) {
      lfree = mem2;
    }
    mem2->used = 0;
    mem2->next = mem->next;
    mem2->prev = ptr;
    mem->next = ptr2;
    if (mem2->next != MEMORY_SIZE_ALIGNED) {
      ((struct mem *)(void *)&ram[mem2->next])->prev = ptr2;
    }
    /* the original mem->next is used, so no need to plug holes! */
  }
  /* else {
    next struct mem is used but size between mem and mem2 is not big enough
    to create another struct mem
    -> don't do anyhting. 
    -> the remaining space stays unused since it is too small
  } */

   memory_free_UNPROTECT();
  return rmem;
}

/**
 * Adam's memory_malloc() plus solution for bug #17922
 * Allocate a block of memory with a minimum of 'size' bytes.
 *
 * @param size is the minimum size of the requested block in bytes.
 * @return pointer to allocated memory or NULL if no free memory was found.
 *
 * Note that the returned value will always be aligned (as defined by MEM_ALIGNMENT).
 */
void *
memory_malloc(mem_size_t size)
{
  mem_size_t ptr, ptr2;
  struct mem *mem, *mem2;

   MEM_ALLOC_DECL_PROTECT();

  if (size == 0) {
    return NULL;
  }

  /* Expand the size of the allocated MEMORY region so that we can
     adjust for alignment. */
  size =  MEMORY_ALIGN_SIZE(size);

  if(size < MEMORY_MIN_SIZE_ALIGNED) {
    /* every data block must be at least MEMORY_MIN_SIZE_ALIGNED long */
    size = MEMORY_MIN_SIZE_ALIGNED;
  }

  if (size > MEMORY_SIZE_ALIGNED) {
    return NULL;
  }

  
   MEM_ALLOC_PROTECT();

    /* Scan through the heap searching for a free block that is big enough,
     * beginning with the lowest free block.
     */
    for (ptr = (mem_size_t)((u8_t *)lfree - ram); ptr < MEMORY_SIZE_ALIGNED - size;
         ptr = ((struct mem *)(void *)&ram[ptr])->next) {
      mem = (struct mem *)(void *)&ram[ptr];

      if ((!mem->used) &&
          (mem->next - (ptr + SIZEOF_STRUCT_MEM)) >= size) {
        /* mem is not used and at least perfect fit is possible:
         * mem->next - (ptr + SIZEOF_STRUCT_MEM) gives us the 'user data size' of mem */

        if (mem->next - (ptr + SIZEOF_STRUCT_MEM) >= (size + SIZEOF_STRUCT_MEM + MEMORY_MIN_SIZE_ALIGNED)) {
          /* (in addition to the above, we test if another struct mem (SIZEOF_STRUCT_MEM) containing
           * at least MEMORY_MIN_SIZE_ALIGNED of data also fits in the 'user data space' of 'mem')
           * -> split large block, create empty remainder,
           * remainder must be large enough to contain MEMORY_MIN_SIZE_ALIGNED data: if
           * mem->next - (ptr + (2*SIZEOF_STRUCT_MEM)) == size,
           * struct mem would fit in but no data between mem2 and mem2->next
           * @todo we could leave out MEMORY_MIN_SIZE_ALIGNED. We would create an empty
           *       region that couldn't hold data, but when mem->next gets freed,
           *       the 2 regions would be combined, resulting in more free MEMORY
           */
          ptr2 = ptr + SIZEOF_STRUCT_MEM + size;
          /* create mem2 struct */
          mem2 = (struct mem *)(void *)&ram[ptr2];
          mem2->used = 0;
          mem2->next = mem->next;
          mem2->prev = ptr;
          /* and insert it between mem and mem->next */
          mem->next = ptr2;
          mem->used = 1;

          if (mem2->next != MEMORY_SIZE_ALIGNED) {
            ((struct mem *)(void *)&ram[mem2->next])->prev = ptr2;
          }
        } else {
          /* (a mem2 struct does no fit into the user data space of mem and mem->next will always
           * be used at this point: if not we have 2 unused structs in a row, plug_holes should have
           * take care of this).
           * -> near fit or excact fit: do not split, no mem2 creation
           * also can't move mem->next directly behind mem, since mem->next
           * will always be used at this point!
           */
          mem->used = 1;
        }

        if (mem == lfree) {
          struct mem *cur = lfree;
          /* Find next free block after mem and update lowest free pointer */
          while (cur->used && cur != ram_end) {
            cur = (struct mem *)(void *)&ram[cur->next];
          }
          lfree = cur;
        }
         MEM_ALLOC_UNPROTECT();
//		ptr_test = (mem_size_t)((u8_t *)lfree - ram);

        return (u8_t *)mem + SIZEOF_STRUCT_MEM;
      }
    }

  MEM_ALLOC_UNPROTECT();
  return NULL;
}


