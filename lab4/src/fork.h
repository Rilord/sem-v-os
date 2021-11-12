#ifndef _FORK_H_
#define _FORK_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

typedef enum result {
    SUCCESS = 0,
    ERROR_ALLOC_FAIL = -1,
    ERROR_FORK_FAIL = -2,
    ERROR_EXEC_FAILED = -3,
    ERROR_PIPE_FAILED = -4,
    ERROR_WRONG_ARGS = -5,
} result;

typedef enum mod { 
    PRINT = 0, 
    QUIET = 1 
} mod;

#endif /* _FORK_H_ */
