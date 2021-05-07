cmake_minimum_required(VERSION 3.19)
project(SFS C)

set(CMAKE_C_STANDARD 11)

add_executable(SFS main.c fs.c fs.h tests.c)