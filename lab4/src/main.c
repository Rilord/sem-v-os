#include <assert.h>
#include "fork.h"

#define CHECK_RESULT(f)\
{\
    result res = (f);\
    if (res != SUCCESS)\
    {\
        fprintf(\
                stderr, \
                "Fatal: Result is %d in \n", res);\
        assert(res == SUCCESS);\
    }\
}

static result check_args(const int argc, const char *argv[], const int num) {
    if (argc < num)
        return ERROR_WRONG_ARGS;


    return SUCCESS;
}

int main(const int argc, const char *argv[]) {

    CHECK_RESULT(check_args(argc, argv, 2));

    if (strcmp(argv[1], "-f") == 0) {
        CHECK_RESULT(check_args(argc, argv, 4));
        CHECK_RESULT(fork_loop(atoi(argv[2]), atoi(argv[3])));
    } else if (strcmp(argv[1], "-e") == 0) {
        char *cmds[2] = { "pwd", "ls -a" };
        CHECK_RESULT(execlp_loop(atoi(argv[2]), atoi(argv[2]), cmds)); 
    } else if (strcmp(argv[1], "-w") == 0) {
        CHECK_RESULT(check_args(argc, argv, 4));
        CHECK_RESULT(wait_loop(atoi(argv[2]), atoi(argv[3])));
    } else if (strcmp(argv[1], "-p") == 0) {
        char *msgs[2] = { "Hello", ",world!" };
        CHECK_RESULT(pipe_loop(2, msgs, 2, 100));
    } else if (strcmp(argv[1], "-m") == 0) {
        char *msgs[2] = { "Hello", ",world!" };
        CHECK_RESULT(pipe_loop(2, msgs, 2, 100));
    }
    

    return SUCCESS;
}
