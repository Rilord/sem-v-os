#include "fork.h"

#define CHILDS_COUNT 100
#define BUFFER_SIZE 1024
#define MSG_COUNT 3

static mod g_mod = QUIET;

static int msg_i = 0;

void sig_change_mod(int signum) { 
    g_mod = !g_mod; 
    msg_i = 0;
}


int main(void) {

    signal(SIGTSTP, sig_change_mod);

    int fd[2];
    pid_t childs[CHILDS_COUNT];
    char buffer[BUFFER_SIZE];

    char* messages[MSG_COUNT] = {"Hello", ", ", "world!"};


    if (pipe(fd) == -1) {
        fprintf(stderr, "Can't pipe\n");
        exit(ERROR_PIPE_FAILED);
    }

    printf("parent born: PID = %d ; PPID = %d ; GROUP = %d\n", getpid(),
            getppid(), getpgrp());

    for (size_t i = 0, msg_i = 0; i < CHILDS_COUNT; ++i, msg_i++, msg_i %= MSG_COUNT) {
        pid_t pid = fork();

        if (pid == -1) {
            fprintf(stderr, "Can't fork\n");
            exit(ERROR_FORK_FAIL);
        } else if (pid == 0) {
            printf("child %zd born: PID = %d ; PPID = %d ; GROUP = %d\n", i,
                    getpid(), getppid(), getpgrp());

            close(fd[0]);
            if (g_mod == PRINT) {
                write(fd[1], messages[msg_i], strlen(messages[msg_i]));
                printf("child %zd send : PID = %d ; MSG = %s\n", i, getpid(),
                        messages[msg_i]);
            } else {
                printf("child %zd send: quiet mode. no message send\n", i);
            }

            exit(SUCCESS);
        } else {
            childs[i] = pid;
        }

        sleep(1);
    }

    for (size_t i = 0; i < CHILDS_COUNT; i++) {
        int status, stat_val = 0;

        printf("parent wait\n");
        pid_t child_pid = wait(&status);

        printf("parent done wait : child process (PID = %d) done. status: %d\n",
                child_pid, status);

        if (WIFEXITED(stat_val)) {
            printf("parent: child process #%zd done with code: %d\n",
                    i + 1, WEXITSTATUS(stat_val));
        } else if (WIFSIGNALED(stat_val)) {
            printf(
                    "parent : child process #%zd finished by signal with code: %d\n",
                    i + 1, WTERMSIG(stat_val)
                  );
        } else if (WIFSTOPPED(stat_val)) {
            printf(
                    "parent: child process #%zd stopped with code: %d\n",
                    i + i, WSTOPSIG(stat_val)
                  );
        }
    }

    close(fd[1]);
    ssize_t read_bytes = read(fd[0], buffer, BUFFER_SIZE);

    if (read_bytes == -1)
        printf("error occured while read\n");

    printf("parent recv: %s\n", buffer);
    printf("parent died : PID = %d ; PPID = %d : GROUP = %d\n", getpid(),
            getppid(), getpgrp());


    return SUCCESS;
}
