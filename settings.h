#pragma once

#include <cstdint>

enum class Network {
    Testnet,
    Betanet,
};

struct Settings {
    Settings(Network network);

    std::uint64_t negativeBalanceAddress;
    bool v100Compatible;
};
