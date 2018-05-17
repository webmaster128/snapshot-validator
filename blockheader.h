#pragma once

#include <cstdint>
#include <string>
#include <vector>

class BlockHeader {

public:
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

    std::vector<unsigned char> serialize();
    std::vector<unsigned char> hash(std::vector<unsigned char> signature = {});
    std::uint64_t id(std::vector<unsigned char> signature);

private:
    std::uint32_t version_;
    std::uint32_t timestamp_;
    std::uint64_t previousBlock_;
    std::uint32_t numberOfTransactions_;
    std::uint64_t totalAmount_;
    std::uint64_t totalFee_;
    std::uint64_t reward_;
    std::uint32_t payloadLength_;
    std::vector<unsigned char> payloadHash_;
    std::vector<unsigned char> generatorPublicKey_;
};
