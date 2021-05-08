cmake_minimum_required(VERSION 3.19)
project(SFS C)

set(CMAKE_C_STANDARD 11)

add_executable(SFS fs.c fs.h tests.c io.c)