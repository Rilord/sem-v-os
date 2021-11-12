#include "fork.h"

#define CHILDS_COUNT 2
#define CHILDS_SLEEP 1

int main(void) {

    pid_t childs[CHILDS_COUNT];

    for (size_t i = 0; i < CHILDS_COUNT; ++i) {
        pid_t pid = fork();

        if (pid == -1) {
            fprintf(stderr, "Couldn't fork\n");
            exit(ERROR_FORK_FAIL);
        } else if (pid == 0) {
            printf(
                    "child %zu born: PID = %d ; PPID = %d ; GROUP = %d\n", 
                    i, getpid(), getppid(), getpgrp()
                  );

            sleep(CHILDS_SLEEP);

            printf(
                    "child %zu died : PID = %d ; PPID = %d ; GROUP = %d\n",
                    i, getpid(), getppid(), getpgrp()
                  );

            exit(SUCCESS);
        } else {
            childs[i] = pid;
        }
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


    printf("parent died : PID = %d ; PPID = %d ; GROUP = %d\n", getpid(),
            getppid(), getpgrp());

    printf("parent died : PID = %d ; PPID = %d ; GROUP = %d\n", getpid(),
            getppid(), getpgrp());

    return SUCCESS; 
}
