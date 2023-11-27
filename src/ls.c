#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>

/* 
    아무것도 입력 안했을 때 : 현재 디렉토리
    argc = 2 일 때는 디렉토리 입력
    argc = 3 옵션 들어감.
*/
void rec(char *dirname) {
    DIR *pdir;
    struct dirent *pde;
    char recdir[1024];

    if ((pdir = opendir(dirname)) == NULL) {
        perror("open dir");
        exit(1);
    }

    printf("\n%s:\n", dirname);

    while ((pde = readdir(pdir))) {
        if (pde->d_name[0] != '.')
            printf("%s\t", pde->d_name);
    }
    closedir(pdir);

    if ((pdir = opendir(dirname)) == NULL) {
        perror("open dir");
        exit(1);
    }
    
    while((pde = readdir(pdir))) {
        if (pde->d_name[0] != '.') {
            if (pde->d_type == 4) {
                sprintf(recdir, "%s/%s", dirname, pde->d_name);
                printf("\n");
                rec(recdir);
            }
        }
    }
    closedir(pdir);

    printf("\n");
}
void ls(char *dirname) {
    int cnt = 0;
    DIR *pdir;
    struct dirent *pde;

    if ((pdir = opendir(dirname)) == NULL) {
        perror("opendir");
        exit(1);
    }

    while ((pde = readdir(pdir)) != NULL) {
        if (strncmp(pde->d_name, ".", 1) == 0) {
            continue;
        }
        
        printf("%s\t", pde->d_name);
        if ((++cnt % 4) == 0)
            printf("\n");
    }

    closedir(pdir);
}
int main(int argc, char *argv[]) {
    if (argc == 1) {
        ls(".");
    }
    else if (argc == 2) {
        if (strcmp(argv[1], "-R") == 0) {
            rec(".");
        }
        else
            ls(argv[1]);
    }
    else if (argc == 3) {
        if (strcmp(argv[1], "-R") == 0){
            rec(argv[2]);
        }
    }

    return 0;
}