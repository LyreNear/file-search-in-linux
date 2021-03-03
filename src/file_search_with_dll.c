#include "include.h"


INT32 search_file(CHAR *, CHAR *);



int main() {
    CHAR *pcPath = "/home/lyre/code/c/file-search-in-linux", *filename = "123";
    search_file(pcPath, filename);
    return 0;
}

INT32 search_file(CHAR *acPath, CHAR *filename) {
    INT32 iRet = 0;
    CHAR acSrcPath[512] = {0};
    struct dirent *pDirent = NULL;
    DIR *pDir = NULL;
    DLL_LIST_S *stSearchDirStack;
    UINT32 u32dirlen = 0;

    stSearchDirStack = (DLL_LIST_S *)MALLOC(sizeof(DLL_LIST_S));
    dll_init(stSearchDirStack);
    snprintf(acSrcPath, sizeof(acSrcPath), "%s", acPath); // current searching path


OPENDIR:
    pDir = opendir(acSrcPath);
    if (!pDir) {
        dir_stack_remove(stSearchDirStack);
        FREE(stSearchDirStack);
        stSearchDirStack = NULL;
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
                iRet = dir_stack_push(stSearchDirStack, pDir, pDirent->d_name);
                if (iRet) {
                    break;
                }
                goto OPENDIR;
            }
        }


        closedir(pDir);
        pDir = NULL;
        if (FALSE == dir_stack_pop(stSearchDirStack, &pDir, &u32dirlen)) {
            break;
        }
        MEMSET(acSrcPath + STRLEN(acSrcPath) - u32dirlen, u32dirlen);
    }


    if (pDir != NULL) {
        closedir(pDir);
    }

    dir_stack_remove(stSearchDirStack);
    FREE(stSearchDirStack);
    stSearchDirStack = NULL;

    return iRet;
}
