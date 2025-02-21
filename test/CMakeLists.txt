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
add_executable(test_filter test_filter.cpp)
target_link_libraries(test_filter EEJ ${GTEST_LIB})
gtest_discover_tests(test_filter)
# -----------------------------------------------------
add_executable(test_join test_join.cpp)
target_link_libraries(test_join EEJ ${GTEST_LIB})
gtest_discover_tests(test_join)
# -----------------------------------------------------
add_executable(test_aggre test_aggre.cpp)
target_link_libraries(test_aggre EEJ ${GTEST_LIB})
gtest_discover_tests(test_aggre)
# -----------------------------------------------------
add_executable(test_ipe_filter test_ipe_filter.cpp)
target_link_libraries(test_ipe_filter EEJ ${GTEST_LIB})
gtest_discover_tests(test_ipe_filter)
# -----------------------------------------------------
add_executable(test_ipe_join test_ipe_join.cpp)
target_link_libraries(test_ipe_join EEJ ${GTEST_LIB})
gtest_discover_tests(test_ipe_join)
# -----------------------------------------------------
add_executable(test_sse_filter test_sse_filter.cpp)
target_link_libraries(test_sse_filter EEJ ${GTEST_LIB})
gtest_discover_tests(test_sse_filter)
# -----------------------------------------------------
