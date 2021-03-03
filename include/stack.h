#ifndef __STACK_H__
#define __STACK_H__



#include "def.h"
#include "dll.h"
#include "include.h"

typedef struct tag_dir_stack_node {
    DLL_NODE_S stDll;
    DIR *pDir ;
    CHAR *pFolder;
} DIR_STACK_NODE_S;


VOID dir_stack_remove(DLL_LIST_S *);
INT32 dir_stack_push(DLL_LIST_S *,DIR *, CHAR *);
BOOL dir_stack_pop(DLL_LIST_S *, DIR **, UINT32 *);

#endif // stack.h
