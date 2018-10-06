cmake_minimum_required(VERSION 3.2)

project(snapshot-validator)

set(ENV{PKG_CONFIG_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/external/libpqxx-installation/lib/pkgconfig:$ENV{PKG_CONFIG_PATH}")
find_package(PkgConfig REQUIRED)
pkg_search_module(PQ REQUIRED libpq)
pkg_search_module(PQXX REQUIRED libpqxx)
pkg_search_module(SODIUM REQUIRED libsodium)
include_directories(${PQ_INCLUDE_DIRS} ${PQXX_INCLUDE_DIRS} ${SODIUM_INCLUDE_DIRS})

message(${PQ_LIBRARIES} ${PQXX_LIBRARIES})

set(CMAKE_CXX_STANDARD 14)

set(MAIN_EXECUTABLE "validate-snapshot-database")
set(TESTS_EXECUTABLE "tests")

add_library(lib
    lib/types.h

    lib/lisk.cpp
    lib/transaction.cpp
)
target_include_directories(lib PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/lib>)
install(TARGETS lib
    EXPORT lib
    ARCHIVE DESTINATION lib
    INCLUDES DESTINATION include
)
install(
    DIRECTORY lib/
    DESTINATION include
    FILES_MATCHING PATTERN "*.h"
)

add_executable(${MAIN_EXECUTABLE}
    src/assets.cpp
    src/blockchain_state.cpp
    src/blockchain_state_validator.cpp
    src/block.h
    src/block.cpp
    src/block_validator.cpp
    src/log.cpp
    src/main.cpp
    src/payload.cpp
    src/summaries.cpp
    src/settings.cpp
    src/transaction_validator.cpp
)
target_link_libraries(${MAIN_EXECUTABLE}
    PUBLIC
        lib
        ${PQXX_LDFLAGS}
        ${PQ_LDFLAGS}

        # Shared library: ${SODIUM_LDFLAGS}
        # Static library: ${SODIUM_LIBDIR}/libsodium.a
        ${SODIUM_LIBDIR}/libsodium.a
)
install(TARGETS ${MAIN_EXECUTABLE}
    RUNTIME DESTINATION bin
)

add_executable(${TESTS_EXECUTABLE}
    tests/main.cpp
    tests/transaction.cpp
)

target_link_libraries(${TESTS_EXECUTABLE}
    PUBLIC
        lib
        ${PQXX_LDFLAGS}
        ${PQ_LDFLAGS}

        # Shared library: ${SODIUM_LDFLAGS}
        # Static library: ${SODIUM_LIBDIR}/libsodium.a
        ${SODIUM_LIBDIR}/libsodium.a
)

enable_testing()
add_test(all_tests ${TESTS_EXECUTABLE})
