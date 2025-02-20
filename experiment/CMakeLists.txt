# Grab all cpp file except the run.
file(GLOB SOURCES "*.cpp")
list(REMOVE_ITEM SOURCES "run.cpp")

# Add executable and link the library.
add_executable(EXP "run.cpp" ${SOURCES})
target_link_libraries(EXP PUBLIC EEJ)
