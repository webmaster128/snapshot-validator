#include "blockheader.h"

#include <algorithm>
#include <assert.h>

#include <sodium.h>

BlockHeader::BlockHeader(
        std::uint32_t _version,
        std::uint32_t _timestamp,
        std::uint64_t _previousBlock,
        std::uint32_t _numberOfTransactions,
        std::uint64_t _totalAmount,
        std::uint64_t _totalFee,
        std::uint64_t _reward,
        std::uint32_t _payloadLength,
        std::vector<unsigned char> _payloadHash,
        std::vector<unsigned char> _generatorPublicKey)
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

std::vector<unsigned char> BlockHeader::serialize() const
{

    std::size_t size = 4 // version
            + 4 // timestamp
            + 8 // previous block
            + 4 // number of transactions
            + 8 // total amount
            + 8 // total fee
            + 8 // reward
            + 4 // payload length
            + 32 // payload hash
            + 32 // generator pubkey
            ;
    auto out = std::vector<unsigned char>(size);
    int index = 0;
    out[index++] = (version >> 0*8) & 0xFF;
    out[index++] = (version >> 1*8) & 0xFF;
    out[index++] = (version >> 2*8) & 0xFF;
    out[index++] = (version >> 3*8) & 0xFF;
    out[index++] = (timestamp >> 0*8) & 0xFF;
    out[index++] = (timestamp >> 1*8) & 0xFF;
    out[index++] = (timestamp >> 2*8) & 0xFF;
    out[index++] = (timestamp >> 3*8) & 0xFF;
    out[index++] = (previousBlock >> 7*8) & 0xFF;
    out[index++] = (previousBlock >> 6*8) & 0xFF;
    out[index++] = (previousBlock >> 5*8) & 0xFF;
    out[index++] = (previousBlock >> 4*8) & 0xFF;
    out[index++] = (previousBlock >> 3*8) & 0xFF;
    out[index++] = (previousBlock >> 2*8) & 0xFF;
    out[index++] = (previousBlock >> 1*8) & 0xFF;
    out[index++] = (previousBlock >> 0*8) & 0xFF;
    out[index++] = (numberOfTransactions >> 0*8) & 0xFF;
    out[index++] = (numberOfTransactions >> 1*8) & 0xFF;
    out[index++] = (numberOfTransactions >> 2*8) & 0xFF;
    out[index++] = (numberOfTransactions >> 3*8) & 0xFF;
    out[index++] = (totalAmount >> 0*8) & 0xFF;
    out[index++] = (totalAmount >> 1*8) & 0xFF;
    out[index++] = (totalAmount >> 2*8) & 0xFF;
    out[index++] = (totalAmount >> 3*8) & 0xFF;
    out[index++] = (totalAmount >> 4*8) & 0xFF;
    out[index++] = (totalAmount >> 5*8) & 0xFF;
    out[index++] = (totalAmount >> 6*8) & 0xFF;
    out[index++] = (totalAmount >> 7*8) & 0xFF;
    out[index++] = (totalFee >> 0*8) & 0xFF;
    out[index++] = (totalFee >> 1*8) & 0xFF;
    out[index++] = (totalFee >> 2*8) & 0xFF;
    out[index++] = (totalFee >> 3*8) & 0xFF;
    out[index++] = (totalFee >> 4*8) & 0xFF;
    out[index++] = (totalFee >> 5*8) & 0xFF;
    out[index++] = (totalFee >> 6*8) & 0xFF;
    out[index++] = (totalFee >> 7*8) & 0xFF;
    out[index++] = (reward >> 0*8) & 0xFF;
    out[index++] = (reward >> 1*8) & 0xFF;
    out[index++] = (reward >> 2*8) & 0xFF;
    out[index++] = (reward >> 3*8) & 0xFF;
    out[index++] = (reward >> 4*8) & 0xFF;
    out[index++] = (reward >> 5*8) & 0xFF;
    out[index++] = (reward >> 6*8) & 0xFF;
    out[index++] = (reward >> 7*8) & 0xFF;
    out[index++] = (payloadLength >> 0*8) & 0xFF;
    out[index++] = (payloadLength >> 1*8) & 0xFF;
    out[index++] = (payloadLength >> 2*8) & 0xFF;
    out[index++] = (payloadLength >> 3*8) & 0xFF;
    for (int i = 0; i < payloadHash.size(); ++i) {
        out[index++] = payloadHash[i];
    }
    for (int i = 0; i < generatorPublicKey.size(); ++i) {
        out[index++] = generatorPublicKey[i];
    }

    assert(index == size);

    return out;
}

std::vector<unsigned char> BlockHeader::hash(std::vector<unsigned char> signature) const
{
    auto out = std::vector<unsigned char>(crypto_hash_sha256_BYTES);

    auto message = serialize();
    message.insert(message.end(), signature.begin(), signature.end());
    crypto_hash_sha256(out.data(), message.data(), message.size());

    return out;
}

std::uint64_t BlockHeader::id(std::vector<unsigned char> signature) const
{
    auto hashBytes = hash(signature);
    auto firstBytes = std::vector<unsigned char>{hashBytes.cbegin(), hashBytes.cbegin()+8};
    std::reverse(firstBytes.begin(), firstBytes.end());

    std::uint64_t out = 0
        | (std::uint64_t{firstBytes[0]} << 7*8)
        | (std::uint64_t{firstBytes[1]} << 6*8)
        | (std::uint64_t{firstBytes[2]} << 5*8)
        | (std::uint64_t{firstBytes[3]} << 4*8)
        | (std::uint64_t{firstBytes[4]} << 3*8)
        | (std::uint64_t{firstBytes[5]} << 2*8)
        | (std::uint64_t{firstBytes[6]} << 1*8)
        | (std::uint64_t{firstBytes[7]} << 0*8);
    return out;
}
