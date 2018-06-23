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

    # Betanet snapshot
    wget -O lisk_beta_backup.gz "http://lisk.prolina.org/snapshots/betanet/lisk_beta_backup-501768.gz"
    sha256sum lisk_beta_backup.gz | grep "0e49769bf6dc68c8b59c9ef9296dd674114c5abd0e2be27623c90d6f71aa47d7"

    # Testnet snapshot
    wget https://testnet-snapshot.lisknode.io/blockchain.db.gz
)

(
    # shellcheck disable=SC2030
    PATH="$(pwd)/build:$PATH"
    ./validate_snapshot.sh betanet snapshots/lisk_beta_backup.gz
    ./validate_snapshot.sh testnet snapshots/blockchain.db.gz
)

(
    mkdir "$HOME/custom_dir" && cd "$HOME/custom_dir"
    npm init --yes
    npm install "$WORKSPACE"

    # shellcheck disable=SC2031
    PATH="$(npm bin):$PATH"
    validate-snapshot-database --help
)
