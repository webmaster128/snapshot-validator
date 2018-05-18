#pragma once

#include <unordered_map>

struct State {
    std::unordered_map<std::uint64_t, std::int64_t> balances;
};

void validateState(const State state);
