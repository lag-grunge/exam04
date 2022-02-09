cmake_minimum_required(VERSION 3.20)
project(microshell C)

set(CMAKE_C_STANDARD 99)

add_executable(microshell microshell.c parser.c utils.c change_dir.c)
