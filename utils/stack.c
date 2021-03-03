#include "stack.h"

INT32 dir_stack_push(DLL_LIST_S *pstack ,DIR *pDir , CHAR *pdirname ) {
    DIR_STACK_NODE_S *pnode = NULL;


    pnode = (DIR_STACK_NODE_S *) MALLOC(sizeof(DIR_STACK_NODE_S));
    if (!pnode) {
        return -1;
    }
    MEMSET(pnode, sizeof(DIR_STACK_NODE_S));
    pnode->pDir = pDir;
    pnode->pFolder = strdup(pdirname);
    if (NULL == pnode->pFolder) {
        FREE(pnode);
        return -1;
    }
    dll_append(pstack, &pnode->stDll);

    return 0;
}

BOOL dir_stack_pop(DLL_LIST_S *pstack, DIR **ppDir , UINT32 *pu32dirlen) {
    DIR_STACK_NODE_S *pnode = NULL;
    DLL_NODE_S *pstDll = NULL;

    pstDll = dll_get_tail(pstack);
    if (!pstDll) {
        return FALSE;
    }
    pnode = DLL_GET_ENTRY(pstDll, DIR_STACK_NODE_S, stDll);
    *ppDir = pnode->pDir;
    if (pnode->pFolder) {
        *pu32dirlen = STRLEN(pnode->pFolder) + 1;
        FREE(pnode->pFolder);
    }
    dll_remove(pstack, pstDll);
    FREE(pnode);

    return TRUE;
}

VOID dir_stack_remove(DLL_LIST_S *pstack) {
    DLL_NODE_S *pstDll = NULL;
    DIR_STACK_NODE_S *pnode = NULL;
    pstDll = dll_pop(pstack);
    while (pstDll) {
        pnode = DLL_GET_ENTRY(pstDll, DIR_STACK_NODE_S, stDll);
        if (pnode->pFolder) {
            FREE(pnode->pFolder);
        }
        if (pnode->pDir) {
            closedir(pnode->pDir);
        }
        FREE(pnode);
        pstDll = dll_pop(pstack);
    }

    return;
}

