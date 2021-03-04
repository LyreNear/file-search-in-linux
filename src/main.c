#include "include.h"

int main(int argc, char **argv) {
    if (3 == argc) {
        search_file(argv[1], argv[2]);
    } else {
        printf("Invalid input! Use < ./file_search abs_path filename > to search for file contains filename in the given absolute dir path.\n");

    }
    return 0;
}