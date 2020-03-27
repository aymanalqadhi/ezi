cmake_minimum_required(VERSION 3.10)

# Project Settings
project(ezi_tests)
include_directories(../include/)

# Add Unity Framework
add_subdirectory(Unity/)

###############################[ Tests Targets ]################################

set(DS_SLIST_TESTS ds_slist_tests)
set(DS_HASH_TABLE_TESTS ds_hash_table_tests)
set(IO_ENDIANNESS_TESTS io_endian_tests)
set(IO_READ_WRITE_TESTS io_read_write)
set(DATA_SOFTWARE_TESTS data_software_tests)
set(DATA_SOFTWARE_SOURCE_TESTS data_software_source_tests)
set(DATA_SOFTWARE_DATABASE_TESTS data_software_database_tests)

# Singly  Linked-List Tests
add_executable(${DS_SLIST_TESTS} ds_slist_tests.c ../src/ds_slist.c
               ../src/util_memory.c)
target_link_libraries(${DS_SLIST_TESTS} unity)

# Hash Table  Tests
add_executable(${DS_HASH_TABLE_TESTS} ds_hash_table_tests.c
                ../src/ds_hash_table.c ../src/util_memory.c ../src/ds_slist.c)
target_link_libraries(${DS_HASH_TABLE_TESTS} unity)

# Endianness tests
add_executable(${IO_ENDIANNESS_TESTS} util_endian_tests.c ../src/io_endian.c)
target_link_libraries(${IO_ENDIANNESS_TESTS} unity)

# Read/Write Tests
add_executable(${IO_READ_WRITE_TESTS} io_read_write_tests.c
                                      ../src/io_read_write.c ../src/io_endian.c)
target_link_libraries(${IO_READ_WRITE_TESTS} unity)

# Software Data Tests
add_executable(${DATA_SOFTWARE_TESTS}
    data_software_tests.c ../src/data_software.c ../src/io_read_write.c
    ../src/io_endian.c ../src/ds_slist.c ../src/util_memory.c)
target_link_libraries(${DATA_SOFTWARE_TESTS} unity)

# Software Data Source Tests
add_executable(${DATA_SOFTWARE_SOURCE_TESTS}
    data_software_source_tests.c ../src/data_software.c
    ../src/data_software_source.c ../src/io_read_write.c ../src/io_endian.c
    ../src/ds_slist.c ../src/ds_hash_table.c ../src/util_memory.c)
target_link_libraries(${DATA_SOFTWARE_SOURCE_TESTS} unity)

# Software Database Tests
add_executable(${DATA_SOFTWARE_DATABASE_TESTS}
    data_software_database_tests.c ../src/data_software.c
    ../src/data_software_database.c ../src/data_software_source.c
    ../src/io_read_write.c ../src/io_endian.c ../src/ds_slist.c
    ../src/ds_hash_table.c ../src/util_memory.c)
target_link_libraries(${DATA_SOFTWARE_DATABASE_TESTS} unity)
