#pragma once

#include <unordered_map>
#include <vector>

#include "settings.h"

struct State {
    std::unordered_map<std::uint64_t, std::int64_t> balances;
    std::unordered_map<std::uint64_t, std::vector<unsigned char>> secondPubkeys;
    std::unordered_map<std::uint64_t, std::string> delegateNames;
};

void validateState(const State state, const Settings &settings);
