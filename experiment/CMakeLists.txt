# -----------------------------------------------------
add_executable(SetupTime setup_time.cpp)
target_link_libraries(SetupTime PUBLIC EEJ)
# -----------------------------------------------------
add_executable(EncTime encryption_time.cpp)
target_link_libraries(EncTime PUBLIC EEJ)
# -----------------------------------------------------