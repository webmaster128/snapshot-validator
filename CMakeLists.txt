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

add_executable(${PROJECT_NAME}
    assets.cpp
    blockchain_state.cpp
    blockchain_state_validator.cpp
    block.cpp
    block_validator.cpp
    lisk.cpp
    log.cpp
    main.cpp
    payload.cpp
    summaries.cpp
    settings.cpp
    transaction.cpp
    transaction_validator.cpp
)
target_link_libraries(${PROJECT_NAME}
    ${PQXX_LDFLAGS}
    ${PQ_LDFLAGS}
    ${SODIUM_LDFLAGS}
)
