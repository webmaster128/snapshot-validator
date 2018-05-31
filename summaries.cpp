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

    std::unordered_map<address_t, std::int64_t> blockchainBalances;
    std::unordered_map<address_t, std::uint64_t> blockchainLastBlockIds;
    std::unordered_map<address_t, bytes_t> blockchainSecondPubkeys;
    std::unordered_map<address_t, std::string> blockchainDelegateNames;
    for (auto addressSummary : blockchainState.addressSummaries) {
        auto address = addressSummary.first;
        auto summary = addressSummary.second;
        blockchainBalances[address] = summary.balance;
        blockchainLastBlockIds[address] = summary.lastBlockId;
        blockchainSecondPubkeys[address] = summary.secondPubkey;
        blockchainDelegateNames[address] = summary.delegateName;
    }

    MemAccountsData memAccounts;

    std::string excludedAddressFilter;
    if (!exceptions.invalidAddresses.empty())
    {
        excludedAddressFilter += "WHERE address NOT IN (";
        bool first = true;
        for (auto &address : exceptions.invalidAddresses) {
            if (!first) excludedAddressFilter += ", ";
            excludedAddressFilter += "'" + address + "'";
            first = false;
        }
        excludedAddressFilter += ")";
    }

    pqxx::result result = db.exec(R"SQL(
      SELECT
          left(address, -1),
          balance,
          coalesce("blockId", '0'),
          coalesce("secondPublicKey", ''::bytea),
          coalesce(username, '')
      FROM mem_accounts
      )SQL"
      + excludedAddressFilter);
    for (auto row : result) {
        int index = 0;
        const auto dbAddress = row[index++].as<std::uint64_t>();
        const auto dbBalance = row[index++].as<std::int64_t>();
        const auto dbBlockId = row[index++].as<std::uint64_t>();
        const auto dbSecondPublicKey = pqxx::binarystring(row[index++]);
        const auto dbUsername = row[index++].as<std::string>();

        memAccounts.balances[dbAddress] = dbBalance;
        memAccounts.lastBlockId[dbAddress] = dbBlockId;
        memAccounts.secondPubkeys[dbAddress] = asVector(dbSecondPublicKey);
        memAccounts.delegateNames[dbAddress] = dbUsername;
    }

    if (memAccounts.balances != blockchainBalances) {
        bool keysMatch = compareKeys(memAccounts.balances, blockchainBalances, true, "mem_accounts.balance", "blockchainBalance");
        if (keysMatch) compareValues(memAccounts.balances, blockchainBalances, true, "mem_accounts.balance", "blockchainBalance");
        throw std::runtime_error("Balances in mem_accounts do not match blockchain state");
    }

    if (memAccounts.secondPubkeys != blockchainSecondPubkeys) {
        compareKeys(memAccounts.secondPubkeys, blockchainSecondPubkeys, true);
        throw std::runtime_error("second pubkeys in mem_accounts do not match blockchain state");
    }

    if (memAccounts.lastBlockId != blockchainLastBlockIds) {
        bool keysMatch = compareKeys(memAccounts.lastBlockId, blockchainLastBlockIds, true, "mem_accounts.blockId", "blockchainLastBlockIds");
        if (keysMatch) compareValues(memAccounts.lastBlockId, blockchainLastBlockIds, true, "mem_accounts.blockId", "blockchainLastBlockIds");
        throw std::runtime_error("Block IDs in mem_accounts do not match blockchain state");
    }

    if (memAccounts.delegateNames != blockchainDelegateNames) {
        bool keysMatch = compareKeys(memAccounts.delegateNames, blockchainDelegateNames, true);
        if (keysMatch) compareValues(memAccounts.delegateNames, blockchainDelegateNames, true);
        throw std::runtime_error("delegate names in mem_accounts do not match blockchain state");
    }
}

}
