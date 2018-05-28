#pragma once

#include <cstdint>
#include <cmath>

#include "types.h"

bytes_t firstEightBytesReversed(const bytes_t &data);
std::uint64_t idFromEightBytes(bytes_t firstBytes);
address_t addressFromPubkey(bytes_t publicKey);
inline std::uint64_t roundFromHeight(std::uint64_t height) {
    return std::ceil(height / 101.0);
}
