#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    int cnt = 0;

    DIR *pdir;
    struct dirent *pde;
    if (argc < 2) {
        fprintf(stderr, "rmdir : missing operand\n");
        exit(1);
    }

    if ((pdir = opendir(argv[1]))  == NULL) {
        perror("opendir");
        exit(1);
    }

    // 디렉토리 파일 수 계산
    while ((pde = readdir(pdir)) != NULL) {
        cnt++;
    }                            

    if (cnt <= 2) {
        if (rmdir(argv[1]) < 0) {
            perror("rmdir");          
            exit(1);                 
        }
    }
    else{
        fprintf(stderr, "Directory not empty\n");
        exit(1);
    }

    closedir(pdir);
}