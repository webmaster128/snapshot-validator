#pragma once

#include <string>
#include <unordered_map>
#include <vector>

#include "blockheader.h"
#include "settings.h"
#include "transaction.h"
#include "types.h"

struct BlockchainState {
    std::unordered_map<address_t, std::int64_t> balances;
    std::unordered_map<address_t, bytes_t> secondPubkeys;
    std::unordered_map<address_t, std::string> delegateNames;
    std::unordered_map<address_t, std::uint64_t> lastBlockId;

    void applyTransaction(const TransactionRow &transactionRow);
    void applyBlock(const BlockHeader &bh, std::uint64_t blockId);
    void validate(const Settings &settings) const;
};
