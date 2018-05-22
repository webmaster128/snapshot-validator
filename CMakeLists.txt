cmake_minimum_required(VERSION 3.2)

project(blockchain-validator)

set(ENV{PKG_CONFIG_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/../libpqxx/lib/pkgconfig:$ENV{PKG_CONFIG_PATH}")
find_package(PkgConfig REQUIRED)
pkg_search_module(PQ REQUIRED libpq)
pkg_search_module(PQXX REQUIRED libpqxx)
pkg_search_module(SODIUM REQUIRED libsodium)
include_directories(${PQ_INCLUDE_DIRS} ${PQXX_INCLUDE_DIRS} ${SODIUM_INCLUDE_DIRS})

message(${PQ_LIBRARIES} ${PQXX_LIBRARIES})

add_executable(${PROJECT_NAME}
    assets.cpp
    blockheader.cpp
    lisk.cpp
    main.cpp
    payload.cpp
    summaries.cpp
    settings.cpp
    state.cpp
    transaction.cpp
    transactionvalidator.cpp
)
target_link_libraries(${PROJECT_NAME}
    ${PQ_LDFLAGS}
    ${PQXX_LDFLAGS}
    ${SODIUM_LDFLAGS}
)
