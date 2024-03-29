cmake_minimum_required(VERSION 3.20)
project("AshBuild")

option(ASH_ENABLE_DEVELOPMENT_ENVIRONMENT "Enables a few specific tweaks to be able to develope ash" OFF)
option(ASH_ENABLE_TESTS "Enable ash tests." OFF)
option(ASH_ENABLE_OBJECTS "Enable ash objects." ON)
option(ASH_ENABLE_LOGGER "Enable ash logger, does require fmt." OFF)
option(ASH_ENABLE_USAGE_OF_EXTERNAL_VENDORS "Enable usage of external Vendors." OFF)

option(ASH_OBJECTS_ERASE_NAMES "Choose to erase the names of the AshObjects." OFF)

if(ASH_ENABLE_DEVELOPMENT_ENVIRONMENT)
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
endif(ASH_ENABLE_DEVELOPMENT_ENVIRONMENT)

if(NOT ASH_ENABLE_USAGE_OF_EXTERNAL_VENDORS)
    add_subdirectory("AshVendors")
endif(NOT ASH_ENABLE_USAGE_OF_EXTERNAL_VENDORS)

add_subdirectory("Ash")

if(ASH_ENABLE_OBJECTS)
    add_subdirectory("AshObjects")
endif(ASH_ENABLE_OBJECTS)

if(ASH_ENABLE_LOGGER)
    add_subdirectory("AshLogger")
endif(ASH_ENABLE_LOGGER)

if(ASH_ENABLE_TESTS)
    enable_testing()
    add_subdirectory("AshTest")
endif(ASH_ENABLE_TESTS)