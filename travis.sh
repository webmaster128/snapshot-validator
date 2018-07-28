#!/bin/bash
set -o errexit -o nounset -o pipefail
which shellcheck > /dev/null && shellcheck "$0"

WORKSPACE="$PWD"

(
    cd external/libpqxx
    ./configure --disable-documentation --prefix="$PWD/../libpqxx-installation" && make -j 4 install
)

(
    mkdir build && cd build
    cmake -DCMAKE_BUILD_TYPE=Release .. && make -j 4
)

(
    mkdir snapshots && cd snapshots

    # Testnet snapshot
    if [[ "$NETWORK" == "testnet" ]]; then
        wget https://testnet-snapshot.lisknode.io/blockchain.db.gz
    else
        wget https://snapshot.lisknode.io/blockchain.db.gz
    fi
)

(
    # shellcheck disable=SC2030
    PATH="$(pwd)/build:$PATH"
    ./validate-snapshot-file "$NETWORK" snapshots/blockchain.db.gz
)

(
    mkdir "$HOME/custom_dir" && cd "$HOME/custom_dir"
    npm init --yes
    npm install "$WORKSPACE"

    # shellcheck disable=SC2031
    PATH="$(npm bin):$PATH"
    validate-snapshot-database --help
)
