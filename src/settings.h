#pragma once

#include <cstdint>
#include <set>
#include <string>
#include <unordered_map>

#include "types.h"

enum class Network {
    Mainnet,
    Testnet,
};

inline Network networkFromName(std::string name) {
    if (name == "mainnet") return Network::Mainnet;
    if (name == "testnet") return Network::Testnet;
    throw std::runtime_error("Unknown network name: '" + name + "'");
}

struct Exceptions {
    std::uint64_t freeTransactionsBlockId; // i.e. genesis block
    std::set<std::uint64_t> invalidTransactionSignature;
    std::set<std::uint64_t> inertTransactions;
    std::set<std::uint64_t> transactionsContainingInvalidRecipientAddress;
    std::set<std::string> invalidAddresses;
    std::set<std::uint64_t> payloadHashMismatch;
    std::unordered_map<std::uint64_t, std::int64_t> balanceAdjustments;
    std::unordered_map<height_t, std::uint64_t> blockRewards;
    std::unordered_map<std::uint64_t, int> rewardsFactor;
    std::unordered_map<std::uint64_t, int> feesFactor;
    std::unordered_map<std::uint64_t, int> feesBonus;
};

struct Settings {
    Settings(Network network);

    std::uint64_t genesisBlock;
    std::uint64_t negativeBalanceAddress;
    height_t rewardOffset;
    height_t rewardDistance;
    bool v100Compatible;
    Exceptions exceptions;
};
