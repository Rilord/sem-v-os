#include "fork.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

static mod mode = QUIET;

result fork_loop(size_t child_count, size_t sleep_count) {
    
    pid_t *childs = calloc(child_count, sizeof(pid_t)); 

    if (childs == NULL)
        exit(ERROR_ALLOC_FAIL);

    for (size_t i = 0; i < child_count; ++i) {
        pid_t pid = fork();

        if (pid == -1) {
            fprintf(stderr, "Couldn't fork\n");
            exit(ERROR_FORK_FAIL);
        } else if (pid == 0) {
            printf(
                    "child %zu born: PID = %d ; PPID = %d ; GROUP = %d\n", 
                    i, getpid(), getppid(), getpgrp()
                  );

            sleep(child_count);

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

    free(childs);

    return SUCCESS;
}


result parent_wait(size_t child_count, pid_t *childs) {

    for (size_t i = 0; i < child_count; i++) {
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

    return SUCCESS;
}

result wait_loop(size_t child_count, size_t sleep_count) {

    pid_t *childs = calloc(child_count, sizeof(pid_t)); 

    if (childs == NULL)
        exit(ERROR_ALLOC_FAIL);

    for (size_t i = 0; i < child_count; ++i) {
        pid_t pid = fork();

        if (pid == -1) {
            fprintf(stderr, "Couldn't fork\n");
            exit(ERROR_FORK_FAIL);
        } else if (pid == 0) {
            printf(
                    "child %zu born: PID = %d ; PPID = %d ; GROUP = %d\n", 
                    i, getpid(), getppid(), getpgrp()
                  );

            sleep(child_count);

            printf(
                    "child %zu died : PID = %d ; PPID = %d ; GROUP = %d\n",
                    i, getpid(), getppid(), getpgrp()
                  );

            exit(SUCCESS);
        } else {
            childs[i] = pid;
        }
    }

    parent_wait(child_count, childs);

    free(childs);

    printf("parent died : PID = %d ; PPID = %d ; GROUP = %d\n", getpid(),
            getppid(), getpgrp());


    return SUCCESS;
}

result execlp_loop(size_t child_count, size_t cmd_count, char **cmds) {

    pid_t *childs = calloc(child_count, sizeof(pid_t)); 

    if (childs == NULL)
        exit(ERROR_ALLOC_FAIL);

    for (size_t i = 0; i < child_count; ++i) {
        pid_t pid = fork();

        if (pid == -1) {
            fprintf(stderr, "Couldn't fork\n");
            exit(ERROR_FORK_FAIL);
        } else if (pid == 0) {
            printf(
                    "child %zu born: PID = %d ; PPID = %d ; GROUP = %d\n", 
                    i, getpid(), getppid(), getpgrp()
                  );

            int res = execlp(cmds[i], cmds[i], (char *) NULL);

            if (res == -1) {
                fprintf(stderr, "exec command failed\n");
                exit(ERROR_EXEC_FAILED);
            }

            exit(SUCCESS);
        } else {
            childs[i] = pid;
        }
    }

    parent_wait(child_count, childs);

    return SUCCESS;
}

result pipe_loop(size_t msg_count, char **messages, size_t child_count, size_t buffer_size) {
    int fd[2];


    pid_t *childs = calloc(child_count, sizeof(pid_t)); 

    if (childs == NULL)
        return(ERROR_ALLOC_FAIL);

    char *buffer = calloc(buffer_size, sizeof(char));

    if (buffer == NULL)
        return(ERROR_ALLOC_FAIL);

    if (pipe(fd) == -1) {
        fprintf(stderr, "Can't pipe\n");
        exit(ERROR_PIPE_FAILED);
    }

    printf("parent born: PID = %d ; PPID = %d ; GROUP = %d\n", getpid(),
            getppid(), getpgrp());

    for (size_t i = 0; i < child_count; ++i) {
        pid_t pid = fork();

        if (pid == -1) {
            fprintf(stderr, "Can't fork\n");
            exit(ERROR_FORK_FAIL);
        } else if (pid == 0) {
            printf("child %zd born: PID = %d ; PPID = %d ; GROUP = %d\n", i,
                    getpid(), getppid(), getpgrp());

            close(fd[0]);
            write(fd[1], messages[i], strlen(messages[i]));
            printf("child %zd send : PID = %d ; MSG = %s\n", i, getpid(),
                    messages[i]);

            exit(SUCCESS);
        } else {
            childs[i] = pid;
        }
    }

    parent_wait(child_count, childs);

    close(fd[1]);
    ssize_t read_bytes = read(fd[0], buffer, buffer_size);

    if (read_bytes == -1)
        printf("error occured while read\n");

    printf("parent recv: %s\n", buffer);
    printf("parent died : PID = %d ; PPID = %d : GROUP = %d\n", getpid(),
            getppid(), getpgrp());

    free(childs);
    free(buffer);

    return SUCCESS;
}

result mod_loop(size_t msg_count, char **messages, size_t child_count, size_t buffer_size) {

    int fd[2];


    pid_t *childs = calloc(child_count, sizeof(pid_t)); 

    if (childs == NULL)
        return(ERROR_ALLOC_FAIL);

    char *buffer = calloc(buffer_size, sizeof(char));

    if (buffer == NULL)
        exit(ERROR_ALLOC_FAIL);

    if (pipe(fd) == -1) {
        fprintf(stderr, "Can't pipe\n");
        exit(ERROR_PIPE_FAILED);
    }

    printf("parent born: PID = %d ; PPID = %d ; GROUP = %d\n", getpid(),
            getppid(), getpgrp());

    for (size_t i = 0; i < child_count; ++i) {
        pid_t pid = fork();

        if (pid == -1) {
            fprintf(stderr, "Can't fork\n");
            exit(ERROR_FORK_FAIL);
        } else if (pid == 0) {
            printf("child %zd born: PID = %d ; PPID = %d ; GROUP = %d\n", i,
                    getpid(), getppid(), getpgrp());

            close(fd[0]);
            if (mode == PRINT) {
                write(fd[1], messages[i], strlen(messages[i]));
                printf("child %zd send : PID = %d ; MSG = %s\n", i, getpid(),
                        messages[i]);
            } else {
                printf("child %zd send: quiet mode. no message send", i);
            }

            exit(SUCCESS);
        } else {
            childs[i] = pid;
        }
    }

    parent_wait(child_count, childs);

    close(fd[1]);
    ssize_t read_bytes = read(fd[0], buffer, buffer_size);

    if (read_bytes == -1)
        printf("error occured while read\n");

    printf("parent recv: %s\n", buffer);
    printf("parent died : PID = %d ; PPID = %d : GROUP = %d\n", getpid(),
            getppid(), getpgrp());

    free(childs);
    free(buffer);

    return SUCCESS;
}
