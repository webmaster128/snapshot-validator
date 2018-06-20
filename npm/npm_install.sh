#!/bin/bash
set -o errexit -o nounset -o pipefail
which shellcheck > /dev/null && shellcheck "$0"

(
    cd external/libpqxx
    ./configure --disable-documentation --prefix="$PWD/../libpqxx-installation"
    make -j 4 install
)

(
    mkdir -p npmbuild && cd npmbuild
    cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX="$PWD/.." ..
    make -j 4
    make install
)
