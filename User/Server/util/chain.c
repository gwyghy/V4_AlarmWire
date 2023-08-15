
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "chain.h"
#include "memory_pool.h"


void chain_init(ChainList *pList, const int type, FreeDataFunc freeDataFunc, \
		CompareFunc compareFunc)
{
	if (pList == NULL)
	{
		return;
	}

	pList->head = NULL;
	pList->tail = NULL;
	pList->type = type;
	pList->freeDataFunc = freeDataFunc;
	pList->compareFunc = compareFunc;

	return;
}

void chain_destroy(ChainList *pList)
{
	ChainNode *pNode;
	ChainNode *pDeleted;
#if OS_CRITICAL_METHOD == 3                      
    OS_CPU_SR  cpu_sr = 0;
#endif

	OS_ENTER_CRITICAL();	
	if (pList == NULL || pList->head == NULL)
	{
		OS_EXIT_CRITICAL();
		return;
	}

	pNode = pList->head;
	while (pNode != NULL)
	{
		pDeleted = pNode;
		pNode = pNode->next;

		freeChainNode(pList, pDeleted);
	}

	pList->head = pList->tail = NULL;
	OS_EXIT_CRITICAL();
}

void freeChainNode(ChainList *pList, ChainNode *pChainNode)
{
#if OS_CRITICAL_METHOD == 3                      
    OS_CPU_SR  cpu_sr = 0;
#endif

	OS_ENTER_CRITICAL();	
//	if (pList->freeDataFunc != NULL)
//	{
//		pList->freeDataFunc(pChainNode->data);
//	}

	mempool_free(MEMORYPOOL_CHAIN_NODE,(void*)pChainNode);
	OS_EXIT_CRITICAL();
}

void freeChainNode_mem(ChainList *pList, ChainNode *pChainNode)
{
#if OS_CRITICAL_METHOD == 3                      
    OS_CPU_SR  cpu_sr = 0;
#endif

	OS_ENTER_CRITICAL();	
	if (pList->freeDataFunc != NULL)
	{
		pList->freeDataFunc(pChainNode->data);
	}

	mempool_free(MEMORYPOOL_CHAIN_NODE,(void*)pChainNode);
	OS_EXIT_CRITICAL();
}

int insertNodePrior(ChainList *pList, void *data)
{
	ChainNode *pNode;
#if OS_CRITICAL_METHOD == 3                      
    OS_CPU_SR  cpu_sr = 0;
#endif
		
	if (pList == NULL)
	{
		return EINVAL;
	}

	OS_ENTER_CRITICAL();
	pNode = (ChainNode *)mempool_malloc(MEMORYPOOL_CHAIN_NODE);
	if (pNode == NULL)
	{
		OS_EXIT_CRITICAL();
		return ENOMEM;
	}
		
	pNode->data = data;
	pNode->next = pList->head;

	pList->head = pNode;
	if (pList->tail == NULL)
	{
		pList->tail = pNode;
	}

	OS_EXIT_CRITICAL();
	return 0;
}

int appendNode(ChainList *pList, void *data)
{
	ChainNode *pNode;
#if OS_CRITICAL_METHOD == 3                      
    OS_CPU_SR  cpu_sr = 0;
#endif
	
	if (pList == NULL)
	{
		return EINVAL;
	}

	OS_ENTER_CRITICAL();
	pNode = (ChainNode *)mempool_malloc(MEMORYPOOL_CHAIN_NODE);
	if (pNode == NULL)
	{
		OS_EXIT_CRITICAL();
		return ENOMEM;
	}
	
	pNode->data = data;
	pNode->next = NULL;

	if (pList->tail != NULL)
	{
		pList->tail->next = pNode;
	}

	pList->tail = pNode;
	if (pList->head == NULL)
	{
		pList->head = pNode;
	}

	OS_EXIT_CRITICAL();
	return 0;
}

int insertNodeAsc(ChainList *pList, void *data)
{
	ChainNode *pNew;
	ChainNode *pNode;
	ChainNode *pPrevious;
#if OS_CRITICAL_METHOD == 3                      
    OS_CPU_SR  cpu_sr = 0;
#endif
		
	if (pList == NULL || pList->compareFunc == NULL)
	{
		return EINVAL;
	}

	OS_ENTER_CRITICAL();
	pNew = (ChainNode *)mempool_malloc(MEMORYPOOL_CHAIN_NODE);
	if (pNew == NULL)
	{
		OS_EXIT_CRITICAL();
		return ENOMEM;
	}
	
	pNew->data = data;

	pPrevious = NULL;
	pNode = pList->head;
	while (pNode != NULL && pList->compareFunc(pNode->data, data) < 0)
	{
		pPrevious = pNode;
		pNode = pNode->next;
	}

	pNew->next = pNode;
	if (pPrevious == NULL)
	{
		pList->head = pNew;
	}
	else
	{
		pPrevious->next = pNew;
	}

	if (pNode == NULL)
	{
		pList->tail = pNew;
	}

	OS_EXIT_CRITICAL();
	return 0;
}

int addNode(ChainList *pList, void *data)
{
	if (pList->type == CHAIN_TYPE_INSERT)
	{
		return insertNodePrior(pList, data);
	}
	else if (pList->type == CHAIN_TYPE_APPEND)
	{
		return appendNode(pList, data);
	}
	else
	{
		return insertNodeAsc(pList, data);
	}
}

void deleteNodeEx(ChainList *pList, ChainNode *pPreviousNode, \
		ChainNode *pDeletedNode)
{
#if OS_CRITICAL_METHOD == 3                      
    OS_CPU_SR  cpu_sr = 0;
#endif	
	OS_ENTER_CRITICAL();
	if (pDeletedNode == pList->head)
	{
		pList->head = pDeletedNode->next;
	}
	else
	{
		pPreviousNode->next = pDeletedNode->next;
	}

	if (pDeletedNode == pList->tail)
	{
		pList->tail = pPreviousNode;
	}

	freeChainNode_mem(pList, pDeletedNode);
	OS_EXIT_CRITICAL();
}

void deleteToNodePrevious(ChainList *pList, ChainNode *pPreviousNode, \
			ChainNode *pDeletedNext)
{
	ChainNode *pNode;
	ChainNode *pDeletedNode;
#if OS_CRITICAL_METHOD == 3                      
    OS_CPU_SR  cpu_sr = 0;
#endif	
	OS_ENTER_CRITICAL();
	
	if (pPreviousNode == NULL)
	{
        pNode = pList->head;
		pList->head = pDeletedNext;
	}
	else
	{
        pNode = pPreviousNode->next;
		pPreviousNode->next = pDeletedNext;
	}

    while (pNode != NULL && pNode != pDeletedNext)
	{
		pDeletedNode = pNode;
		pNode = pNode->next;
	    freeChainNode(pList, pDeletedNode);
	}

	if (pDeletedNext == NULL)
	{
		pList->tail = pPreviousNode;
	}
	OS_EXIT_CRITICAL();
}

void *chain_pop_head(ChainList *pList)
{
	ChainNode *pDeletedNode;
	void *data;
#if OS_CRITICAL_METHOD == 3                      
    OS_CPU_SR  cpu_sr = 0;
#endif	
	OS_ENTER_CRITICAL();	

	pDeletedNode = pList->head; 
	if (pDeletedNode == NULL)
	{
		OS_EXIT_CRITICAL();
		return NULL;
	}

	pList->head = pDeletedNode->next;
	if (pList->head == NULL)
	{
		pList->tail = NULL;
	}

	data = pDeletedNode->data;
	mempool_free(MEMORYPOOL_CHAIN_NODE,(void*)pDeletedNode);

	OS_EXIT_CRITICAL();
	return data;
}

int chain_count(ChainList *pList)
{
	ChainNode *pNode;
	int count;
#if OS_CRITICAL_METHOD == 3                      
    OS_CPU_SR  cpu_sr = 0;
#endif	
	OS_ENTER_CRITICAL();
	
	count = 0;
	pNode = pList->head;
	while (pNode != NULL)
	{
		count++;
		pNode = pNode->next;
	}

	OS_EXIT_CRITICAL();
	return count;
}

int deleteNode(ChainList *pList, void *data, char bDeleteAll)
{
	ChainNode *pNode;
	ChainNode *pPrevious;
	ChainNode *pDeleted;
	int nCount;
	int nCompareRes;
#if OS_CRITICAL_METHOD == 3                      
    OS_CPU_SR  cpu_sr = 0;
#endif			

	if (pList == NULL || pList->compareFunc == NULL)
	{
		return -EINVAL;
	}

	OS_ENTER_CRITICAL();
	nCount = 0;
	pPrevious = NULL;
	pNode = pList->head;
	while (pNode != NULL)
	{
		nCompareRes = pList->compareFunc(pNode->data, data);
		if (nCompareRes == 0)
		{
			pDeleted = pNode;
			pNode = pNode->next;

			deleteNodeEx(pList, pPrevious, pDeleted);
			nCount++;

			if (!bDeleteAll)
			{
				break;
			}
			continue;
		}
		else if(nCompareRes > 0 && pList->type == CHAIN_TYPE_SORTED)
		{
			break;
		}

		pPrevious = pNode;
		pNode = pNode->next;
	}

	OS_EXIT_CRITICAL();
	return nCount;
}

int deleteOne(ChainList *pList, void *data)
{
	return deleteNode(pList, data, 0);
}

int deleteAll(ChainList *pList, void *data)
{
	return deleteNode(pList, data, 1);
}

