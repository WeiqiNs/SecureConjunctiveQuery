# Set cmake version.
cmake_minimum_required(VERSION 3.22)

# Project name and language.
project(
        EqualJoin
        VERSION 0.1
        DESCRIPTION "Some description should be here."
        LANGUAGES CXX
)

# Set standards and include tests.
if (CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    set(CMAKE_CXX_STANDARD 20)
    set_property(GLOBAL PROPERTY USE_FOLDERS ON)
    include(CTest)
endif ()

# Find the SSL library.
find_package(OpenSSL REQUIRED)
# Find the relic library.
find_library(RELIC_LIB relic)

# Add desired subdirectories.
add_subdirectory(src)
add_subdirectory(tests)
add_subdirectory(experiments)