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
void redirect(int narg, char *argv[], int index);
void pipeline(char *argv[], int narg, int index);

int main() {
    char buf[256];
    char *argv[50];
    char path[256];
    int narg;

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
    명령줄에 특정 기호가 있는 지 확인하는 함수
    기호의 인덱스를 index에 저장
*/
int check(char ch, char *argv[], int *index, int narg) {
    int i;
    for (i = 0; i < narg; i++) {
        if (strcmp(argv[i], &ch) == 0) {
            *index = i;
            return 1;
        }
    }
    return 0;
}

void redirect(int narg, char *argv[], int index){
    int fd;
    if (strcmp(argv[index], "<") == 0) {
        if ((fd = open(argv[index + 1], O_RDONLY | O_CREAT, 0644)) == -1) {
            perror("open");
            exit(1);
        }
        dup2(fd, STDIN_FILENO);

        // 리다이렉트 기호와 파일 이름을 argv에서 삭제 후 뒤 원소들을 앞으로 당김.
        memmove(argv + index, argv + index + 2, (narg - index - 1) * sizeof(char *));
        argv[narg] = NULL;
        close(fd);
    }
    else if (strcmp(argv[index], ">") == 0) {
        if ((fd = open(argv[index + 1], O_WRONLY | O_CREAT | O_TRUNC, 0644)) == -1) {
            perror("open");
            exit(1);
        }
        dup2(fd, STDOUT_FILENO);

        // 리다이렉트 기호와 파일 이름을 argv에서 삭제 후 뒤 원소들을 앞으로 당김.
        memmove(argv + index, argv + index + 2, (narg - index - 1) * sizeof(char *));
        argv[narg] = NULL;
        close(fd);
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

void pipeline(char *argv[], int narg, int index) {
    int p[2];
    char *first_argv[50]; // 파이프 해야할 때 두 번째 명령어 저장
    pid_t pid;

    memmove(first_argv, argv, index * sizeof(char *)); // 앞 명령어 new_arg에 저장
    first_argv[index] = NULL;
    memmove(argv, argv + index + 1, (narg - index - 1) * sizeof(char *)); // argv에 뒷 명령어만 남게 채움.
    argv[narg - index - 1] = NULL;

    if (pipe(p) == -1) {
        perror("pipe call failed");
        exit(1);
    }

    pid = fork();
    if (pid == 0) {
        dup2(p[1], STDOUT_FILENO);
        execv(first_argv[0], first_argv);
    }
    else if (pid > 0) {
        // close(p[1]);
        dup2(p[0], STDIN_FILENO);
    }
    else {
        perror("fork failed");
        exit(1);
    }
}

int execute_cmd(int narg, char *argv[]) {
    int status; 
    int is_redirect = 0, is_pipe = 0, is_background = 0;
    int index[3];
    pid_t pid;

    is_redirect = check('<', argv, &index[0], narg) || check('>', argv, &index[0], narg);
    is_pipe = check('|', argv, &index[1], narg);
    is_background = check('&', argv, &index[2], narg);

    pid = fork();
    if (pid == 0) {
        if (is_pipe == 1) {
            pipeline(argv, narg, index[1]);
        }
        if (is_redirect == 1) 
            redirect(narg, argv, index[0]);
        printf("%d", getpid());
        execv(argv[0], argv);
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