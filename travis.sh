#!/bin/bash
set -o errexit -o nounset -o pipefail
which shellcheck > /dev/null && shellcheck "$0"

WORKSPACE="$PWD"

BETANET_SNAPSHOT_URL="http://lisk.prolina.org/snapshots/betanet/lisk_beta_backup-551460.gz"
BETANET_SNAPSHOT_SHA256="99669b247595642d8a9d5699142b158fe4177e00e3a322c51005453b0784bf17"

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

    # Betanet snapshot
    wget -O lisk_beta_backup.gz "$BETANET_SNAPSHOT_URL"
    sha256sum lisk_beta_backup.gz | grep "$BETANET_SNAPSHOT_SHA256"

    # Testnet snapshot
    wget https://testnet-snapshot.lisknode.io/blockchain.db.gz
)

(
    # shellcheck disable=SC2030
    PATH="$(pwd)/build:$PATH"
    ./validate-snapshot-file betanet snapshots/lisk_beta_backup.gz
    ./validate-snapshot-file testnet snapshots/blockchain.db.gz
)

(
    mkdir "$HOME/custom_dir" && cd "$HOME/custom_dir"
    npm init --yes
    npm install "$WORKSPACE"

    # shellcheck disable=SC2031
    PATH="$(npm bin):$PATH"
    validate-snapshot-database --help
)
