# Enable Google Test framework.
include(GoogleTest)

# Add all the testing files.
file(GLOB TEST_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

# Create test executables and register them.
foreach (EACH_SOURCE IN LISTS TEST_SOURCES)
    get_filename_component(TEST_NAME ${EACH_SOURCE} NAME_WE)
    add_executable(${TEST_NAME} ${EACH_SOURCE})
    target_link_libraries(${TEST_NAME}
            PRIVATE
            SCQ
            GTest::gtest
            GTest::gtest_main
    )
    gtest_discover_tests(${TEST_NAME})

    # Set folder for IDEs.
    set_target_properties(${TEST_NAME} PROPERTIES FOLDER "Tests")
endforeach ()
