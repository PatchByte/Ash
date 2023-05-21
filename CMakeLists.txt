cmake_minimum_required(VERSION 3.24)
project("AshBuild")

option(ASH_ENABLE_DEVELOPMENT_ENVIRONMENT "Enables a few specific tweaks to be able to develope ash" OFF)
option(ASH_ENABLE_TESTS "Enable ash tests." OFF)
option(ASH_ENABLE_OBJECTS "Enable ash objects." ON)

if(ASH_ENABLE_DEVELOPMENT_ENVIRONMENT)
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
endif(ASH_ENABLE_DEVELOPMENT_ENVIRONMENT)

if(ASH_ENABLE_TESTS)
    add_subdirectory("AshTest")
endif(ASH_ENABLE_TESTS)

add_subdirectory("Ash")


if(ASH_ENABLE_OBJECTS)
    add_subdirectory("AshObjects")
endif(ASH_ENABLE_OBJECTS)
