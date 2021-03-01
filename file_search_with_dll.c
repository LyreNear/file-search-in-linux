#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include "dll.h"
#include <dirent.h>


#define INT32 int32_t
#define UINT32 uint32_t
#define CHAR char
#define BOOL INT32
#define FALSE 0
#define TRUE 1
#define MEMSET(PTR, SIZE) memset((PTR), 0, (SIZE))
#define STRLEN(str) strlen(str)
#define MALLOC(SIZE) malloc((SIZE))
#define FREE(PTR) free((PTR))
#define STRSTR(str1, str2) strstr(str1, str2)


INT32 search_file(CHAR *, CHAR *);
BOOL is_dir(const CHAR *);
BOOL is_file(const CHAR *);
VOID dpm_fs_dir_stack_remove(DLL_LIST_S *);
INT32 dpm_fs_dir_stack_push(DLL_LIST_S *,DIR *, CHAR *);
BOOL dpm_fs_dir_stack_pop(DLL_LIST_S *, DIR **, UINT32 *);




int main() {
    search_file("/home/lyre/code/c/file-search-in-linux", "123");
    return 0;
}

VOID dpm_fs_dir_stack_remove(DLL_LIST_S *pstack)
{
    DLL_NODE_S *pstDll = NULL;
	DPM_DIR_STACK_NODE_S *pnode = NULL;
    pstDll = dll_pop(pstack);
    while (pstDll)
    {
        pnode = DLL_GET_ENTRY(pstDll, DPM_DIR_STACK_NODE_S, stDll);
		if(pnode->pFolder)
		{
			FREE(pnode->pFolder);
		}
		if(pnode->pDir)
		{
			closedir(pnode->pDir);
		}
        FREE(pnode);
        pstDll = dll_pop(pstack);
    }

    return;
}

INT32 dpm_fs_dir_stack_push(DLL_LIST_S *pstack ,DIR *pDir , CHAR *pdirname )
{
	DPM_DIR_STACK_NODE_S *pnode = NULL;


    pnode = (DPM_DIR_STACK_NODE_S *)MALLOC(sizeof(DPM_DIR_STACK_NODE_S));
    if (!pnode)
    {
        return -1;
    }
    MEMSET(pnode, sizeof(DPM_DIR_STACK_NODE_S));
	pnode->pDir = pDir;
	pnode->pFolder = strdup(pdirname);
	if(NULL == pnode->pFolder)
	{
		FREE(pnode);
		return -1;
	}
    dll_append(pstack, &pnode->stDll);

	return 0;
}

BOOL dpm_fs_dir_stack_pop(DLL_LIST_S *pstack, DIR **ppDir , UINT32 *pu32dirlen)
{
	DPM_DIR_STACK_NODE_S *pnode = NULL;
	DLL_NODE_S *pstDll = NULL;

	pstDll = dll_get_tail(pstack);
	if(!pstDll)
	{
		return FALSE;
	}
	pnode = DLL_GET_ENTRY(pstDll, DPM_DIR_STACK_NODE_S, stDll);
	*ppDir = pnode->pDir;
	if(pnode->pFolder)
	{
		*pu32dirlen = STRLEN(pnode->pFolder)+1;
		FREE(pnode->pFolder);
	}
	dll_remove(pstack, pstDll);
	FREE(pnode);

	return TRUE;
}

BOOL is_dir(const CHAR *pcPath)
{
    struct stat stStatbuf;
    if(lstat(pcPath, &stStatbuf) == 0)
    {
        return S_ISDIR(stStatbuf.st_mode) != 0;
    }
    return FALSE;
}

BOOL is_file(const CHAR *pcPath)
{
    struct stat stStatbuf;
    if(lstat(pcPath, &stStatbuf) == 0) {
        return S_ISREG(stStatbuf.st_mode) != 0;
    }
    return FALSE;
}

INT32 search_file(CHAR *acPath, CHAR *filename) {
    INT32 i32Ret = 0;
    CHAR acSrcPath[512] = {0};
    struct dirent *pDirent = NULL;
    DIR *pDir = NULL;
    DLL_LIST_S stDirStack;
    UINT32 u32dirlen = 0;

    snprintf(acSrcPath, sizeof(acSrcPath), "%s", acPath); // current searching path

OPENDIR:
    pDir = opendir(acSrcPath);
    if (!pDir) {
        dir_stack_remove(stDirStack);
        return -1;
    }

    while (TRUE) {
        if (NULL != (pDirent = readdir(pDir))) {
            if (is_special_dir(pDirent->d_name)) {
                continue;
            }

            if (STRLEN(acSrcPath) + STRLEN(pDirent->d_name) >= sizeof(acSrcPath)) {
                iRet = -1;
                break;
            }

            snprintf(acSrcPath + STRLEN(acSrcPath), sizeof(acSrcPath) - STRLEN(acSrcPath), "/%s", pDirent->d_name);

            if (STRSTR(pDirent->d_name, filename)) {
                printf("%s matched, absPath is %s\n", pDirent->d_name, acSrcPath);
            }

            if (is_file(acSrcPath)) {
                MEMSET(acSrcPath + STRLEN(acSrcPath) - STRLEN(pDirent->d_name) - 1, STRLEN(pDirent->d_name) + 1);
                continue;
            } else if (is_dir(acSrcPath)) {
                iRet = dir_stack_push(stDirStack, pDir, pDirent->d_name);
                if (iRet) {
                    break;
                }
                goto OPENDIR;
            }
        }
        closedir(pDir);
        pDir = NULL;
        if (FALSE == dir_stack_pop(stDirStack, &pDir, &u32dirlen)) {
            break;
        }
        MEMSET(acSrcPath + STRLEN(acSrcPath) - u32dirlen, u32dirlen);
    }

exit:
    if (pDir != NULL) {
        closedir(pDir);
    }

    dir_stack_remove(stDirStack);

    return iRet;
}
