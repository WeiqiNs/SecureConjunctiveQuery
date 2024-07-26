# -----------------------------------------------------
add_executable(test_crypto test_crypto.cpp)
target_link_libraries(test_crypto PRIVATE ej_lib)
add_test(NAME test_crypto COMMAND test_crypto)
# -----------------------------------------------------

# -----------------------------------------------------
add_executable(test_helper test_helper.cpp)
target_link_libraries(test_helper PRIVATE ej_lib)
add_test(NAME test_helper COMMAND test_helper)
# -----------------------------------------------------

# -----------------------------------------------------
add_executable(test_init test_init.cpp)
target_link_libraries(test_init PRIVATE ej_lib)
add_test(NAME test_init COMMAND test_init)
# -----------------------------------------------------

# -----------------------------------------------------
add_executable(test_field test_field.cpp)
target_link_libraries(test_field PRIVATE ej_lib)
add_test(NAME test_field COMMAND test_field)
# -----------------------------------------------------

# -----------------------------------------------------
add_executable(test_group test_group.cpp)
target_link_libraries(test_group PRIVATE ej_lib)
add_test(NAME test_group COMMAND test_group)
# -----------------------------------------------------

# -----------------------------------------------------
add_executable(test_filter test_filter.cpp)
target_link_libraries(test_filter PRIVATE ej_lib)
add_test(NAME test_filter COMMAND test_filter)
# -----------------------------------------------------

# -----------------------------------------------------
add_executable(test_aggre test_aggre.cpp)
target_link_libraries(test_aggre PRIVATE ej_lib)
add_test(NAME test_aggre COMMAND test_aggre)
# -----------------------------------------------------