cmake_minimum_required(VERSION 3.1.6)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)



project(lab4 C)

add_executable(fork src/fork.c)
add_executable(wait src/wait.c)
add_executable(exec src/exec.c)
add_executable(pipe src/pipe.c)
add_executable(handler src/handler.c)
add_executable(sort src/sort.c)
add_executable(matrix_multiplication src/mult.c)
