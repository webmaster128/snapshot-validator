#include "summaries.h"

#include <cstdint>
#include <iostream>
#include <string>
#include <unordered_map>

#include "scopedbenchmark.h"
#include "utils.h"

namespace {

struct MemAccountsData {
    std::unordered_map<address_t, std::int64_t> balances;
    std::unordered_map<address_t, std::vector<unsigned char>> secondPubkeys;
    std::unordered_map<address_t, std::string> delegateNames;
    std::unordered_map<address_t, std::uint64_t> lastBlockId;
};

}

namespace Summaries {

void checkMemAccounts(pqxx::read_transaction &db, const BlockchainState &blockchainState, const Exceptions &exceptions)
{
    std::cout << "Checking mem_accounts ..." << std::endl;
    ScopedBenchmark benchmarkMemAccounts("Checking mem_accounts"); static_cast<void>(benchmarkMemAccounts);

    MemAccountsData memAccounts;

    std::string listOfExcludedAddresses;
    for (auto &address : exceptions.invalidAddresses) {
        if (!listOfExcludedAddresses.empty()) listOfExcludedAddresses += ", ";
        listOfExcludedAddresses += "'" + address + "'";
    }

    pqxx::result result = db.exec(R"SQL(
      SELECT
          left(address, -1), balance, "blockId",
          "secondPublicKey", coalesce(username, '')
      FROM mem_accounts
      WHERE address NOT IN (
        )SQL" + listOfExcludedAddresses + R"SQL(
      )
    )SQL");
    for (auto row : result) {
        int index = 0;
        const auto dbAddress = row[index++].as<std::uint64_t>();
        const auto dbBalance = row[index++].as<std::int64_t>();
        const auto dbBlockId = row[index++].as<std::uint64_t>();
        const auto dbSecondPublicKey = pqxx::binarystring(row[index++]);
        const auto dbUsername = row[index++].as<std::string>();

        memAccounts.balances[dbAddress] = dbBalance;
        memAccounts.lastBlockId[dbAddress] = dbBlockId;
        if (dbSecondPublicKey.length() != 0) memAccounts.secondPubkeys[dbAddress] = asVector(dbSecondPublicKey);
        if (!dbUsername.empty()) memAccounts.delegateNames[dbAddress] = dbUsername;
    }

    if (memAccounts.balances != blockchainState.balances) {
        bool keysMatch = compareKeys(memAccounts.balances, blockchainState.balances, true);
        if (keysMatch) compareValues(memAccounts.balances, blockchainState.balances, true);
        throw std::runtime_error("Balances in mem_accounts do not match blockchain state");
    }

    if (memAccounts.secondPubkeys != blockchainState.secondPubkeys) {
        compareKeys(memAccounts.secondPubkeys, blockchainState.secondPubkeys, true);
        throw std::runtime_error("second pubkeys in mem_accounts do not match blockchain state");
    }

    if (memAccounts.lastBlockId != blockchainState.lastBlockId) {
        bool keysMatch = compareKeys(memAccounts.lastBlockId, blockchainState.lastBlockId, true);
        if (keysMatch) compareValues(memAccounts.lastBlockId, blockchainState.lastBlockId, true);
        throw std::runtime_error("Block IDs in mem_accounts do not match blockchain state");
    }

    if (memAccounts.delegateNames != blockchainState.delegateNames) {
        bool keysMatch = compareKeys(memAccounts.delegateNames, blockchainState.delegateNames, true);
        if (keysMatch) compareValues(memAccounts.delegateNames, blockchainState.delegateNames, true);
        throw std::runtime_error("delegate names in mem_accounts do not match blockchain state");
    }
}

}
