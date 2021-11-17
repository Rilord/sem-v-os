#include "fork.h"
#include <stdio.h>
#include <string.h>
#include <math.h>

#define CHILDS_COUNT 4
#define BUFFER_SIZE 2048
#define MSG "01"
#define MSG_LEN 2

static mod g_mod = PRINT;

static void sig_change_mod(int signum) { 
    g_mod = g_mod == PRINT ? QUIET : PRINT;
}

static void free_strings_array(char **arr, int n) {
    for (size_t i = 0; i < n; i++)
        free(arr[i]);
    free(arr);
}

static char *repeat_string(const char *str, int count) {
    char *ret = malloc(MSG_LEN * (count = (int) pow(2, count)));
    if (ret == NULL)
        return NULL;
    strcpy(ret, str);
    while (--count > 0) {
        strcat(ret, str);
    }
    return ret;
}

static char **create_messages() {
    char **arr = (char **) malloc(CHILDS_COUNT * sizeof(char *));
    if (arr == NULL)
        return NULL;
    for (size_t i = 0; i < CHILDS_COUNT; ++i) {
        arr[i] = (char *) repeat_string(MSG, i);
        if (arr[i] == NULL) {
            free_strings_array(arr, i);
            return NULL;
        }
    }

    return arr;
}

int main(void) {

    signal(SIGTSTP, sig_change_mod);

    int fd[2];
    pid_t childs[CHILDS_COUNT];
    char buffer[BUFFER_SIZE];

    char **messages = create_messages();

    if (messages == NULL)
        exit(ERROR_ALLOC_FAIL);

    if (pipe(fd) == -1) {
        fprintf(stderr, "Can't pipe\n");
        exit(ERROR_PIPE_FAILED);
    }

    printf("parent born: PID = %d ; PPID = %d ; GROUP = %d\n", getpid(),
            getppid(), getpgrp());

    for (size_t i = 0, msg = 0; i < CHILDS_COUNT; ++i, msg+= 1 * g_mod, sleep(2)) {
        pid_t pid = fork();

        if (pid == -1) {
            fprintf(stderr, "Can't fork\n");
            exit(ERROR_FORK_FAIL);
        } else if (pid == 0) {
            printf("child %zd born: PID = %d ; PPID = %d ; GROUP = %d\n", i,
                    getpid(), getppid(), getpgrp());

            if (g_mod == PRINT) {
                close(fd[0]);
                write(fd[1], messages[msg], strlen(messages[msg]));
                printf("child %zu sent : PID = %d ; MSG = %s\n", i, getpid(),
                        messages[msg]);
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

    printf("parent recv: %s (%zu)\n", buffer, strlen(buffer));
    printf("parent died : PID = %d ; PPID = %d : GROUP = %d\n", getpid(),
            getppid(), getpgrp());

    free_strings_array(messages, CHILDS_COUNT);

    return SUCCESS;
}
