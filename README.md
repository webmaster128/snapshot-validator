Snapshot validator for Lisk
===========================

Double check your snapshots!

A tool to validate the data integrety of a Lisk snapshot.

**Security note**: even though this tool checks a lot of data consistency, it is far from perfect.
Thus a validated snapshot can still contain corrupted data. Thus keep choosing snapshot
providers wisely.

## Design principles

* Read-only (does not produce or change snapshots)
* Fast runtime (validates 10k blocks/second on a 4 year old laptop)
* Independent development (to avoid copying bugs)

## Prerequirements

* A postgres server must be running
* `createdb test_database_xyz` must work for the current user

## How to install/compile

* Install libpq-dev, libsodium-dev, a C++ compiler and cmake using your favourite package manager
* `git clone --recursive https://github.com/prolina-foundation/snapshot-validator.git`
* `cd snapshot-validator/external/libpqxx`
* `./configure --disable-documentation --prefix=$PWD/../libpqxx-installation && make -j 4 install`
* `cd ../..`
* `mkdir build && cd build`
* `cmake -DCMAKE_BUILD_TYPE=Release .. && make -j 4`
* Now move the resulting binary `snapshot-validator` into PATH, e.g. `sudo mv snapshot-validator /usr/local/bin`

## How to use

* Ensure `snapshot-validator` is in PATH: `snapshot-validator --help`
* Run `./validate_snapshot.sh testnet <testnet snapshot file>`

## Further notes

* Temporary databases are not dropped when validation fails. Use a Postgres management tool
  like e.g. pgAdmin to clean up from time to time.
