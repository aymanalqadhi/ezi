cmake_minimum_required(VERSION 3.10)

# Project Settings
project(ezi_tests)
include_directories(../include/)

# Add Unity Framework
add_subdirectory(Unity/)

###############################[ Tests Targets ]################################

set(DS_SLIST_TESTS ds_slist_tests)
set(DS_HASH_TABLE_TESTS ds_hash_table_tests)

# Singly  Linked-List Tests
add_executable(${DS_SLIST_TESTS} ds_slist_tests.c ../src/ds_slist.c
               ../src/util_memory.c)
target_link_libraries(${DS_SLIST_TESTS} unity)

# Hash Table  Tests
add_executable(${DS_HASH_TABLE_TESTS} ds_hash_table_tests.c
                ../src/ds_hash_table.c ../src/util_memory.c ../src/ds_slist.c)
target_link_libraries(${DS_HASH_TABLE_TESTS} unity)
