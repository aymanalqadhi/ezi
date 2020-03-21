cmake_minimum_required(VERSION 3.10)

# Project Settings
project(ezi_tests)
include_directories(../include/)

# Add Unity Framework
add_subdirectory(Unity/)

# Tests Targets
add_executable(ds_slist_tests ds_slist_tests.c ../src/ds_slist.c ../src/memory.c)
target_link_libraries(ds_slist_tests unity)
