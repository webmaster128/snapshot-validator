#pragma once

#include <cstdint>
#include <set>
#include <unordered_map>

enum class Network {
    Testnet,
    Betanet,
};

struct Exceptions {
    std::set<std::uint64_t> invalidTransactionSignature;
    std::set<std::uint64_t> inertTransactions;
    std::set<std::uint64_t> recipientAddressOutOfRange;
    std::unordered_map<std::uint64_t, std::int64_t> transactionFees;
    std::unordered_map<std::uint64_t, std::int64_t> balanceAdjustments;
};

struct Settings {
    Settings(Network network);

    std::uint64_t negativeBalanceAddress;
    bool v100Compatible;
    Exceptions exceptions;
};
