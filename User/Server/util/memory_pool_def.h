

#ifndef MEMORYPOOL_CHAIN_NUM
#define MEMORYPOOL_CHAIN_NUM                100
#endif


/*
 * A list of internal pools used by LWIP.
 *
 *  MEMORYPOOL(pool_name, number_elements, element_size, pool_description)
 *     creates a pool name MEMP_pool_name. description is used in stats.c
 */

 MEMORYPOOL(CHAIN_NODE,        MEMORYPOOL_CHAIN_NUM,         sizeof(void *)*2,        "CHAIN_NODE")


#undef  MEMORYPOOL
