# Set cmake version.
cmake_minimum_required(VERSION 3.20)

# Project name and language.
project(
        SecureConjunctiveQuery
        DESCRIPTION "This project implements a few methods to process conjunctive queries securely over an encrypted database."
        LANGUAGES CXX
)

# Set standards.
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Enable the testing.
enable_testing()

# Find required libraries.
find_package(RBP REQUIRED)
find_package(OpenSSL REQUIRED)

# Find the google test library for testing.
find_package(GTest)
if (NOT GTest_FOUND)
    include(FetchContent)
    FetchContent_Declare(
            googletest
            URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
            DOWNLOAD_EXTRACT_TIMESTAMP TRUE
    )
    # For Windows: Prevent overriding the parent project's compiler/linker settings
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)
endif ()

# Add desired subdirectories.
add_subdirectory(src)
add_subdirectory(test)
add_subdirectory(experiment)