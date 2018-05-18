#pragma once

#include <cstdint>
#include <string>
#include <vector>

struct BlockHeader {
    BlockHeader(
        std::uint32_t version,
        std::uint32_t timestamp,
        std::uint64_t previousBlock,
        std::uint32_t numberOfTransactions,
        std::uint64_t totalAmount,
        std::uint64_t totalFee,
        std::uint64_t reward,
        std::uint32_t payloadLength,
        std::vector<unsigned char> payloadHash,
        std::vector<unsigned char> generatorPublicKey
    );

    const std::uint32_t version;
    const std::uint32_t timestamp;
    const std::uint64_t previousBlock;
    const std::uint32_t numberOfTransactions;
    const std::uint64_t totalAmount;
    const std::uint64_t totalFee;
    const std::uint64_t reward;
    const std::uint32_t payloadLength;
    const std::vector<unsigned char> payloadHash;
    const std::vector<unsigned char> generatorPublicKey;

    std::vector<unsigned char> serialize();
    std::vector<unsigned char> hash(std::vector<unsigned char> signature = {});
    std::uint64_t id(std::vector<unsigned char> signature);
};
