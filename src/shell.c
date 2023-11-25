#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>

void cmd_cd(int argc, char *argv[]);
int getargs(char *cmd, char **argv);
int execute_cmd(int narg, char *args[]);
void redirect(int narg, char *argv[]);

int main() {
    char buf[256];
    char *argv[50];
    char path[256];
    int narg;
    pid_t pid;

    while(1) {
        printf("shell> ");
        fgets(buf, 256, stdin);
        clearerr(stdin);
        narg = getargs(buf, argv);

        if (strcmp(argv[0], "exit") == 0) { 
            return 0;
        }
        else if (strcmp(argv[0], "cd") == 0) {
            cmd_cd(narg, argv);
            printf("%s\n", getcwd(path, 256));
        } 
        else {
            execute_cmd(narg, argv);
        }


    }
}

int getargs(char *cmd, char *argv[]) {
    int narg = 0;


    while (*cmd) {
        if (*cmd == ' ' || *cmd == '\t' || *cmd == '\n')
            *cmd++ = '\0';
        else {
            argv[narg++] = cmd++;
            while (*cmd != '\0' && *cmd != ' ' && *cmd != '\t' && *cmd != '\n')
                cmd++;
        }
    }



    argv[narg] = NULL;
    return narg;
}

/*
리다이렉트 명령이 있을 시 리다이렉트
*/
void redirect(int narg, char *argv[]){
    int fd, i = 0;
    for (i = 0; i < narg; i++) {
        if (strcmp(argv[i], "<") == 0) {
            if ((fd = open(argv[i + 1], O_RDONLY | O_CREAT, 0644)) == -1) {
                perror("open");
                exit(1);
            }
            dup2(fd, STDIN_FILENO);

            // 리다이렉트 기호와 파일 이름을 argv에서 삭제
            memmove(argv + i, argv + i + 2, (narg - i - 1) * sizeof(char *));
            argv[narg] = NULL;
            close(fd);

            break;
        }
        if (strcmp(argv[i], ">") == 0) {
            if ((fd = open(argv[i + 1], O_WRONLY | O_CREAT | O_TRUNC, 0644)) == -1) {
                perror("open");
                exit(1);
            }
            dup2(fd, STDOUT_FILENO);

            // 리다이렉트 기호와 파일 이름을 argv에서 삭제
            memmove(argv + i, argv + i + 2, (narg - i - 1) * sizeof(char *));
            argv[narg] = NULL;
            close(fd);

            break;
        }
    }
}
void cmd_cd(int argc, char *argv[]) {
    if (argc == 1) {
        argv[1] = getenv("HOME");
    } else if (argc > 2) {
        fprintf(stderr, "too many arguments\n");
        return;
    }

    if (chdir(argv[1]) < 0) {
        perror("cd");
        return;
    }
}
int execute_cmd(int narg, char *args[]) {
    int status;
    pid_t pid;

    pid = fork();
    if (pid == 0) {
        redirect(narg, args);
        execv(args[0], args); 
        perror("exec");
        exit(1);
    }
    else if (pid >0) {
        waitpid(pid, &status, 0); 
    }
    else
        perror("fork failed");
    
    return 0;
}