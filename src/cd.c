#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

/*
    입력하지 않았을 때
    . ..
    ~ /
*/

int main(int argc, char *argv[]) {
    if (argc == 1) {
        argv[1] = getenv("HOME");
    } else if (argc > 2) {
        fprintf(stderr, "too many arguments\n");
        exit(1);
    }

    if (chdir(argv[1]) < 0) {
        perror("cd");
        exit(1);
    }

    return 0;
}