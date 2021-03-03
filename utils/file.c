#include "file.h"

BOOL is_dir(const CHAR *pcPath) {
    struct stat stStatbuf;
    if (lstat(pcPath, &stStatbuf) == 0) {
        return S_IFDIR & stStatbuf.st_mode;
    }
    return FALSE;
}

BOOL is_file(const CHAR *pcPath) {
    struct stat stStatbuf;
    if (lstat(pcPath, &stStatbuf) == 0) {
        return S_IFREG & stStatbuf.st_mode;
    }
    return FALSE;
}

BOOL is_special_dir(const CHAR *pcPath) {
    if (strcmp(pcPath, ".") && strcmp(pcPath, "..")) {
        return FALSE;
    }
    return TRUE;
}

