#!/bin/bash
set -o errexit -o nounset -o pipefail
which shellcheck > /dev/null && shellcheck "$0"

(
    cd external/libpqxx
    ./configure --disable-documentation --prefix="$PWD/../libpqxx-installation" && make -j 4 install
)

(
    mkdir build && cd build
    cmake -DCMAKE_BUILD_TYPE=Release .. && make -j 4
)
