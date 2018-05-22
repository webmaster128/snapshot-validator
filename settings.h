#pragma once

#include <cstdint>
#include <set>

enum class Network {
    Testnet,
    Betanet,
};

struct Exceptions {
    std::set<std::uint64_t> invalidTransactionSignature;
};

struct Settings {
    Settings(Network network);

    std::uint64_t negativeBalanceAddress;
    bool v100Compatible;
    Exceptions exceptions;
};
