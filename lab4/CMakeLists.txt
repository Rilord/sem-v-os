cmake_minimum_required(VERSION 3.1.6)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)



project(lab4 C)

add_executable(lab src/main.c src/fork.c src/fork.h)
