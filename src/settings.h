#pragma once

#include <cstdint>
#include <map>
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

struct Settings {
    Settings(Network network);

    std::uint64_t genesisBlock;
    std::uint64_t negativeBalanceAddress;
    height_t rewardOffset;
    height_t rewardDistance;
    std::map<height_t, std::uint32_t> blockVersions;
    Exceptions exceptions;
};
