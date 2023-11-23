#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <math.h>

int getdirnames(char **argv, char **dirnames) {
    int cnt = 0;
    while ((dirnames[cnt] = strtok(argv[2], "/")) != NULL) { 
        if (strcmp(dirnames[cnt], ".") == 0)
            continue;
        cnt++; 
    }
    return cnt;
}
void makedir(char *argv, mode_t mode) {
    if (mkdir(argv, mode) < 0) {
        perror("mkdir");
        exit(1);
    }
}
int main(int argc, char *argv[]) {
    int i;
    char ch;
    char *dirnames[256];
    mode_t mode = 0777, mask;

    if (argc == 2) {
        makedir(argv[1], mode);
    }
    else if (argc == 4) {
        if (strcmp(argv[1], "-m") == 0) {
            if ((strcmp(argv[2], "000") >= 0) && (strcmp(argv[2], "777") <= 0)) {
                mask = umask(0000);
                mode = 0;
                // 권한 문자열을 8진수로 변환
                for (i = 0; i < 3; i++) {
                    ch = argv[2][i];
                    mode += atoi(&ch) * (int)pow(8, 2 - i);
                }
                makedir(argv[3], mode);
                // 원래의 마스크 복구
                umask(mask);
            }
            else {
                fprintf(stderr, "invalid mode %s\n", argv[2]);
                exit(1);
            }
        }
        else {
            fprintf(stderr, "mkdir: invalid option -- %s\n", argv[1]);
            fprintf(stderr, "가능한 옵션 ; -m\n");
            exit(1);
        }
    }
    return 0;
}