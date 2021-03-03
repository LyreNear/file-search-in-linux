#ifndef DLL_H
#define DLL_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdint.h>
#include "def.h"

#ifndef INLINE
    #ifdef __APPLE__
        #define INLINE extern inline
    #elif __linux__
        #define INLINE __inline
    #else
        #define INLINE
    #endif
#endif

#ifdef __cplusplus
extern "C" {
#endif

#define DLL_FOR_EACH(pLIST, e)  \
    for (DLL_NODE_S * e = (pLIST)->pstHead; e != NULL; e = dll_get_next((pLIST), e)) \

#define DLL_FOR_EACH_TAIL(pLIST, e)  \
    for (DLL_NODE_S * e = (pLIST)->pstTail; e != NULL; e = dll_get_prev((pLIST), e)) \

#define DLL_FOR_EACH_SAFE(list, pos, n)  \
    for (DLL_NODE_S *pos = (list)->pstHead, *n = pos?dll_get_next((list), pos):NULL, *__i = NULL;\
         pos != NULL && (n != (list)->pstHead || __i == NULL); \
         pos = n, n = pos?dll_get_next((list), pos):NULL, __i ++) \

#define DLL_FOR_EACH_BEGIN(pLIST, STRUCT, e, MEMBER)    \
do {    \
    DLL_NODE_S * _d_tmp = dll_get_first(pLIST); \
    while (_d_tmp != NULL)   \
    {   \
        e = DLL_GET_ENTRY(_d_tmp, STRUCT, MEMBER);   \
        _d_tmp = dll_get_next(pLIST, _d_tmp);


#define DLL_FOR_EACH_END()  \
    }   \
}   \
while (0)

#define DLL_GET_ENTRY(PLLNODE, STRUCT, MEMBER)    \
    (STRUCT *)((char *)PLLNODE - (char *)&(((STRUCT *)0)->MEMBER))

#define DLL_NOT_IN_LIST(pDLL_NODE)   \
    ((pDLL_NODE)->pstNext == (pDLL_NODE)->pstPrev && (pDLL_NODE)->pstPrev == NULL)

#define DLL_INIT_NODE(node) \
{ \
    (node)->pstList = NULL; \
    (node)->pstPrev = (node)->pstNext = NULL; \
}

typedef struct tag_dll_node
{
    struct tag_dll_node *pstPrev;
    struct tag_dll_node *pstNext;
    struct tag_dll_list *pstList;
}DLL_NODE_S;

typedef struct tag_dll_list
{
    uint32_t uiCount;
    struct tag_dll_node *pstHead;
    struct tag_dll_node *pstTail;
}DLL_LIST_S;

#define DLL_IS_ISOLATE(dll) ((dll)->pstNext == NULL && (dll)->pstPrev == NULL)

void dll_init(DLL_LIST_S *pstList);
BOOL dll_is_empty(DLL_LIST_S *pstList);
void dll_append(DLL_LIST_S *pstList, DLL_NODE_S *pstNode);
extern void dll_remove(DLL_LIST_S *pstList, DLL_NODE_S *pstNode);
DLL_NODE_S* dll_get_first(DLL_LIST_S *pstList);
DLL_NODE_S* dll_get_tail(DLL_LIST_S *pstList);
DLL_NODE_S* dll_get_prev(DLL_LIST_S *pstList, DLL_NODE_S *pstNode);
DLL_NODE_S* dll_get_next(DLL_LIST_S *pstList, DLL_NODE_S *pstNode);
DLL_NODE_S* dll_next(DLL_NODE_S *pstNode);
uint32_t dll_get_count(DLL_LIST_S *pstList);
uint32_t dll_is_in_list(DLL_NODE_S *pstDll, DLL_LIST_S *pstList);
DLL_NODE_S* dll_pop(DLL_LIST_S *pstList);
DLL_NODE_S* dll_cut(DLL_LIST_S *pstList);
void dll_insert_after(DLL_NODE_S *pstCurr, DLL_NODE_S *pstNode);
void dll_insert_before(DLL_NODE_S *pstCurr, DLL_NODE_S *pstNode);

#ifdef __cplusplus
}
#endif

#endif //DLL_H
