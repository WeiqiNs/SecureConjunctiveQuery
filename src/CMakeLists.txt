# Get all header and source files.
file(GLOB SOURCE_LIST CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/src/*.cpp")

# Make an automatic library - will be static or dynamic based on user setting.
add_library(EEJ ${SOURCE_LIST})

# We need this directory, and users of our library will need it too.
target_include_directories(EEJ PUBLIC ${PROJECT_SOURCE_DIR}/include)

# Link the executable with the library.
target_link_libraries(EEJ PUBLIC OpenSSL::Crypto RBP::RBP)
