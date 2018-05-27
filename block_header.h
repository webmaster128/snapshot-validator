#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include "types.h"

struct BlockHeader {
    BlockHeader(
        const std::uint32_t _version,
        const std::uint32_t _timestamp,
        const std::uint64_t _previousBlock,
        const std::uint32_t _numberOfTransactions,
        const std::uint64_t _totalAmount,
        const std::uint64_t _totalFee,
        const std::uint64_t _reward,
        const std::uint32_t _payloadLength,
        const bytes_t _payloadHash,
        const bytes_t _generatorPublicKey)
        : version(_version)
        , timestamp(_timestamp)
        , previousBlock(_previousBlock)
        , numberOfTransactions(_numberOfTransactions)
        , totalAmount(_totalAmount)
        , totalFee(_totalFee)
        , reward(_reward)
        , payloadLength(_payloadLength)
        , payloadHash(_payloadHash)
        , generatorPublicKey(_generatorPublicKey)
    {
    }

    const std::uint32_t version;
    const std::uint32_t timestamp;
    const std::uint64_t previousBlock;
    const std::uint32_t numberOfTransactions;
    const std::uint64_t totalAmount;
    const std::uint64_t totalFee;
    const std::uint64_t reward;
    const std::uint32_t payloadLength;
    const bytes_t payloadHash;
    const bytes_t generatorPublicKey;

    bytes_t serialize() const;
    bytes_t hash(bytes_t signature = {}) const;
    std::uint64_t id(bytes_t signature) const;
};
