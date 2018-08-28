Snapshot validator for Lisk
===========================

[![Build Status](https://travis-ci.com/prolina-foundation/snapshot-validator.svg?branch=master)](https://travis-ci.com/prolina-foundation/snapshot-validator)
[![npm version](https://img.shields.io/npm/v/snapshot-validator.svg)](https://www.npmjs.com/package/snapshot-validator)

Double check your snapshots!

A tool to validate the data integrity of a Lisk snapshot.

**Security note**: even though this tool checks a lot of data consistency, it is far from perfect.
Thus a validated snapshot can still contain corrupted data. Thus keep choosing snapshot
providers wisely.

## Design principles

* Read-only (does not produce or change snapshots)
* Fast runtime (validates 10k blocks/second on a 4 year old laptop)
* Independent development (to avoid copying bugs)
* Return code is king: 0 for success, everything else for error (output text is debugging only)

## Prerequirements

* A postgres server must be running
* `createdb test_database_xyz` must work for the current user

## How to install/compile

* Install libpq-dev, libsodium-dev, pkg-config, cmake, a C++ compiler, and Python, using your favourite package manager.
* `git clone --recursive https://github.com/prolina-foundation/snapshot-validator.git`
* `cd snapshot-validator/external/libpqxx`
* `./configure --disable-documentation --prefix="$PWD/../libpqxx-installation" && make -j 4 install`
* `cd ../..`
* `mkdir build && cd build`
* `cmake -DCMAKE_BUILD_TYPE=Release .. && make -j 4`
* Now move the resulting binary `validate-snapshot-database` into PATH, e.g. `sudo mv validate-snapshot-database /usr/local/bin`

## How to use

* Ensure `validate-snapshot-database` is in PATH: `validate-snapshot-database --help`
* Run `./validate-snapshot-file testnet <testnet snapshot file>`

## Further notes

* Temporary databases are not dropped when validation fails. Use a Postgres management tool
  like e.g. pgAdmin to clean up from time to time.
* Mainnet validation consumes 4.5 GiB of memory on Linux because all transactions are kept in memory

## License

MIT
