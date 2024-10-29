# Get all header and source files.
file(GLOB HEADER_LIST CONFIGURE_DEPENDS "${EqualJoin_SOURCE_DIR}/include/*.hpp")
file(GLOB SOURCE_LIST CONFIGURE_DEPENDS "${EqualJoin_SOURCE_DIR}/src/*.cpp")

# Make an automatic library - will be static or dynamic based on user setting.
add_library(ej_lib ${SOURCE_LIST} ${HEADER_LIST})

# We need this directory, and users of our library will need it too.
target_include_directories(ej_lib PUBLIC ${EqualJoin_SOURCE_DIR}/include)

# This depends on the relic library we found.
target_link_libraries(ej_lib PRIVATE OpenSSL::Crypto)
target_link_libraries(ej_lib PRIVATE ${RELIC_LIB})