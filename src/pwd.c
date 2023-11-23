#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define MAX_SIZE 256

int main() {
    char pwd[MAX_SIZE];

    if (getcwd(pwd, MAX_SIZE) == NULL) {
        perror("pwd");
        exit(1);
    }
    printf("%s\n", pwd);
    
    return 0;
}