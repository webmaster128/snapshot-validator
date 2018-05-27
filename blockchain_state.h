#pragma once

#include <string>
#include <unordered_map>
#include <vector>

#include "settings.h"
#include "transaction.h"
#include "types.h"

struct BlockchainState {
    std::unordered_map<address_t, std::int64_t> balances;
    std::unordered_map<address_t, bytes_t> secondPubkeys;
    std::unordered_map<address_t, std::string> delegateNames;

    void applyTransaction(const Transaction &transaction);
    void validate(const Settings &settings) const;
};
