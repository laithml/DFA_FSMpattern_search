cmake_minimum_required(VERSION 3.23)
project(ex1 C)

set(CMAKE_C_STANDARD 99)

add_executable(ex1 slist.h pattern_matching.h slist.c pattern_matching.c main.c)
