cmake_minimum_required(VERSION 3.17)
project(lab9 C)

set(CMAKE_C_STANDARD 99)

add_executable(lab9 main.c graph_adapter.c graph_adapter.h graph.h btree.c btree.h graph_reader.c graph_reader.h)