# -----------------------------------------------------
# Include google test here and enable testing.
include(GoogleTest)
# Set alias for google test.
set(GTEST_LIB gtest gtest_main)
# -----------------------------------------------------
add_executable(test_helper test_helper.cpp)
target_link_libraries(test_helper EEJ ${GTEST_LIB})
gtest_discover_tests(test_helper)
# -----------------------------------------------------
add_executable(test_crypto test_crypto.cpp)
target_link_libraries(test_crypto EEJ ${GTEST_LIB})
gtest_discover_tests(test_crypto)
# -----------------------------------------------------
