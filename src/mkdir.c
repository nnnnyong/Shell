#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>

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
    int i, len;
    mode_t mode = 0777, mask;

    if (argc == 2) {
        makedir(argv[1], mode);
    }
    else if (argc == 4) {
        if (strcmp(argv[1], "-m") == 0) {
            mask = umask(0000); // 마스크 해제
            mode = 0;
            len = strlen(argv[2]);
            // 권한 문자열을 8진수로 변환
            for (i = 0; i < len; i++) {
                if (argv[2][i] < '0' && argv[2][i] > 7){
                    fprintf(stderr, "invalid mode %s\n", argv[2]);
                    exit(1);
                }
                mode = mode * 8 + (argv[2][i] - '0');
            }
            makedir(argv[3], mode);
	    umask(mask);
        }
        else {
            fprintf(stderr, "mkdir: invalid option -- %s\n", argv[1]);
            fprintf(stderr, "가능한 옵션 ; -m\n");
            exit(1);
        }
    }
    return 0;
}
