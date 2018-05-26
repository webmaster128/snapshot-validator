#pragma once

#include <cstdint>
#include <set>
#include <unordered_map>

enum class Network {
    Testnet,
    Betanet,
};

inline Network networkFromName(std::string name) {
    if (name == "testnet") return Network::Testnet;
    if (name == "betanet") return Network::Betanet;
    throw std::runtime_error("Unknown network name: '" + name + "'");
}

struct Exceptions {
    std::set<std::uint64_t> invalidTransactionSignature;
    std::set<std::uint64_t> inertTransactions;
    std::set<std::uint64_t> transactionsContainingInvalidRecipientAddress;
    std::set<std::string> invalidAddresses;
    std::set<std::uint64_t> payloadHashMismatch;
    std::unordered_map<std::uint64_t, std::int64_t> transactionFees;
    std::unordered_map<std::uint64_t, std::int64_t> balanceAdjustments;
};

struct Settings {
    Settings(Network network);

    std::uint64_t negativeBalanceAddress;
    bool v100Compatible;
    Exceptions exceptions;
};
