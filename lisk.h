#pragma once

#include <cstdint>
#include <vector>

std::vector<unsigned char> firstEightBytesReversed(const std::vector<unsigned char> &data);
std::uint64_t idFromEightBytes(std::vector<unsigned char> firstBytes);
std::uint64_t addressFromPubkey(std::vector<unsigned char> publicKey);
