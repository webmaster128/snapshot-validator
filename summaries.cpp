#include "summaries.h"

#include <cstdint>
#include <iostream>
#include <string>

#include "scopedbenchmark.h"
#include "utils.h"

namespace Summaries {

void checkMemAccounts(pqxx::read_transaction &db, const State &blockchainState)
{
    std::cout << "Checking mem_accounts ..." << std::endl;
    ScopedBenchmark benchmarkMemAccounts("Checking mem_accounts"); static_cast<void>(benchmarkMemAccounts);

    std::unordered_map<std::uint64_t, std::int64_t> balances;
    pqxx::result result = db.exec(R"SQL(
        SELECT
            left(address, -1), balance
        FROM mem_accounts
    )SQL");
    for (auto row : result) {
        int index = 0;
        const auto dbAddress = row[index++].as<std::uint64_t>();
        const auto dbBalance = row[index++].as<std::int64_t>();
        balances[dbAddress] = dbBalance;
    }

    if (balances != blockchainState.balances) {
        // debug
        compareKeys(blockchainState.balances, balances, true);
        throw std::runtime_error("Balances in mem_accounts do not match blockchain state");
    }
}

}
