#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int getargs(char *cmd, char **argv);

int main() {
    char buf[256];
    char *argv[50];
    int narg;
    pid_t pid;

    while(1) {
        printf("shell> ");
        fgets(buf, 256, stdin);
        clearerr(stdin);
        narg = getargs(buf, argv);

        pid = fork();
        if (pid == 0)
            execvp(argv[0], argv);
        else if (pid >0)
            wait((int *)0);
        else
            perror("fork failed");
    }
}

int getargs(char *cmd, char **argv) {
    int narg = 0;

    while (*cmd) {
        if (*cmd == ' ' || *cmd == '\n')
            *cmd++ = '\0';
        else {
            argv[narg++] = cmd++;
            while (*cmd != '\0' && *cmd != ' ' && *cmd != '\n')
                cmd++;
        }
    }

    argv[narg] = NULL;
    return narg;
}