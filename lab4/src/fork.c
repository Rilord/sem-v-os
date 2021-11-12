#include "fork.h"

#define CHILDS_COUNT 2
#define CHILDS_SLEEP 2
#define BUF_SIZE   128

int main(void) {

    pid_t childs[CHILDS_COUNT];
    char   buf[BUF_SIZE];

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

    printf("parent died : PID = %d ; PPID = %d ; GROUP = %d\n", getpid(),
            getppid(), getpgrp());

    return SUCCESS; 
}
