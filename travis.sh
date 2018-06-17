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

(
    mkdir snapshots && cd snapshots
    wget http://lisk.prolina.org/snapshots/betanet/lisk_beta_backup-482679.gz
    sha256sum lisk_beta_backup-482679.gz | grep "5916434690d506c31cb9637b9477185d9707e2fd5c6939d9c9e56a263bdb1019"
)

(
    PATH="$(pwd)/build:$PATH"
    ./validate_snapshot.sh betanet snapshots/lisk_beta_backup-482679.gz
)
