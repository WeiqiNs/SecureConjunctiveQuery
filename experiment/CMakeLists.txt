# Grab all cpp file except the run.
file(GLOB SOURCES "*.cpp")
list(REMOVE_ITEM SOURCES "run.cpp")

# Add executable and link the library.
add_executable(EXP "run.cpp" ${SOURCES})
target_link_libraries(EXP PUBLIC EEJ)

# Move experimental data to compiled location if exists.
if (EXISTS ${CMAKE_SOURCE_DIR}/experiment/data)
    file(COPY ${CMAKE_SOURCE_DIR}/experiment/data DESTINATION ${CMAKE_BINARY_DIR}/experiment)
endif ()