#include "dll.h"

INLINE void dll_init(DLL_LIST_S *pstList)
{
    pstList->uiCount = 0;
    pstList->pstHead = NULL;
    pstList->pstTail = NULL;
}

INLINE BOOL dll_is_empty(DLL_LIST_S *pstList)
{
    return (pstList->pstHead == pstList->pstTail && pstList->pstHead == NULL);
}

void dll_append(DLL_LIST_S *pstList, DLL_NODE_S *pstNode)
{
    if (pstList == pstNode->pstList)
    {
        return;
    }
    if (dll_is_empty(pstList))
    {
        pstList->pstHead = pstNode;
        pstList->pstTail = pstNode;
        pstNode->pstNext = pstNode;
        pstNode->pstPrev = pstNode;
    }
    else
    {
        pstList->pstTail->pstNext = pstNode;
        pstNode->pstNext = pstList->pstHead;
        pstNode->pstPrev = pstList->pstTail;
        pstList->pstHead->pstPrev = pstNode;
        pstList->pstTail = pstNode;
    }
    pstNode->pstList = pstList;
    pstList->uiCount++;
}

void dll_insert_after(DLL_NODE_S *pstCurr, DLL_NODE_S *pstNode)
{
    DLL_LIST_S *pstList = pstCurr->pstList;

    if (pstCurr->pstList == pstNode->pstList)
    {
        return;
    }
    pstNode->pstPrev = pstCurr;
    pstNode->pstNext = pstCurr->pstNext;
    pstCurr->pstNext->pstPrev = pstNode;
    pstCurr->pstNext = pstNode;

    if (pstList->pstTail == pstCurr)
    {
        pstList->pstTail = pstNode;
    }
    if (pstList->pstHead == pstCurr && pstList->pstTail == pstNode)
    {
        pstCurr->pstPrev = pstNode;
    }

    pstNode->pstList = pstList;
    pstList->uiCount++;
}

void dll_insert_before(DLL_NODE_S *pstCurr, DLL_NODE_S *pstNode)
{
    DLL_LIST_S *pstList = pstCurr->pstList;

    if (pstCurr->pstList == pstNode->pstList)
    {
        return;
    }
    pstNode->pstNext = pstCurr;
    pstNode->pstPrev = pstCurr->pstPrev;
    pstCurr->pstPrev->pstNext= pstNode;
    pstCurr->pstPrev = pstNode;

    if (pstList->pstHead == pstCurr)
    {
        pstList->pstHead = pstNode;
    }
    if (pstList->pstTail == pstCurr && pstList->pstHead == pstNode)
    {
        pstCurr->pstNext = pstNode;
    }

    pstNode->pstList = pstList;
    pstList->uiCount++;
}

void dll_remove(DLL_LIST_S *pstList, DLL_NODE_S *pstNode)
{
    if (dll_is_empty(pstList))
    {
        return;
    }
    if (pstList != pstNode->pstList)
    {
        return;
    }
    if (pstNode->pstNext == NULL && pstNode->pstPrev == NULL)
    {
        return;
    }
    pstNode->pstPrev->pstNext = pstNode->pstNext;
    pstNode->pstNext->pstPrev = pstNode->pstPrev;
    if ((pstNode == pstList->pstTail) && (pstNode == pstList->pstHead))
    {
        pstList->pstHead = NULL;
        pstList->pstTail = NULL;
    }
    else if (pstNode == pstList->pstHead)
    {
        pstList->pstHead = pstNode->pstNext;
    }
    else if (pstNode == pstList->pstTail)
    {
        pstList->pstTail = pstNode->pstPrev;
    }
    pstNode->pstPrev = pstNode->pstNext = NULL;
    pstNode->pstList = NULL;
    //pstList->uiCount--;
    __sync_sub_and_fetch(&pstList->uiCount, 1);
}

INLINE DLL_NODE_S* dll_get_first(DLL_LIST_S *pstList)
{
    return pstList->pstHead;
}

INLINE DLL_NODE_S* dll_get_tail(DLL_LIST_S *pstList)
{
    return pstList->pstTail;
}

INLINE DLL_NODE_S* dll_get_next(DLL_LIST_S *pstList, DLL_NODE_S *pstNode)
{
    return (pstNode != pstList->pstTail && pstNode->pstList == pstList) ? pstNode->pstNext : NULL;
}

INLINE DLL_NODE_S* dll_next(DLL_NODE_S *pstNode)
{
    return pstNode->pstNext;
}

INLINE DLL_NODE_S* dll_get_prev(DLL_LIST_S *pstList, DLL_NODE_S *pstNode)
{
    return (pstNode != pstList->pstHead) ? pstNode->pstPrev : NULL;
}

INLINE uint32_t dll_get_count(DLL_LIST_S *pstList)
{
    return pstList->uiCount;
}

INLINE uint32_t dll_is_in_list(DLL_NODE_S *pstDll, DLL_LIST_S *pstList)
{
    return (pstList == pstDll->pstList);
}

DLL_NODE_S* dll_pop(DLL_LIST_S *pstList)
{
    DLL_NODE_S *pstRet = pstList->pstHead;

    if (pstList->uiCount == 0 || NULL == pstList->pstHead)
    {
        return NULL;
    }

    dll_remove(pstList, pstList->pstHead);
    return pstRet;
}


