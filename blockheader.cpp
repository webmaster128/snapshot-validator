#include "blockheader.h"

#include <algorithm>
#include <assert.h>

#include <sodium.h>

BlockHeader::BlockHeader(
        std::uint32_t version,
        std::uint32_t timestamp,
        std::uint64_t previousBlock,
        std::uint32_t numberOfTransactions,
        std::uint64_t totalAmount,
        std::uint64_t totalFee,
        std::uint64_t reward,
        std::uint32_t payloadLength,
        std::vector<unsigned char> payloadHash,
        std::vector<unsigned char> generatorPublicKey)
    : version_(version)
    , timestamp_(timestamp)
    , previousBlock_(previousBlock)
    , numberOfTransactions_(numberOfTransactions)
    , totalAmount_(totalAmount)
    , totalFee_(totalFee)
    , reward_(reward)
    , payloadLength_(payloadLength)
    , payloadHash_(payloadHash)
    , generatorPublicKey_(generatorPublicKey)
{
}

std::vector<unsigned char> BlockHeader::serialize()
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
    out[index++] = (version_ >> 0*8) & 0xFF;
    out[index++] = (version_ >> 1*8) & 0xFF;
    out[index++] = (version_ >> 2*8) & 0xFF;
    out[index++] = (version_ >> 3*8) & 0xFF;
    out[index++] = (timestamp_ >> 0*8) & 0xFF;
    out[index++] = (timestamp_ >> 1*8) & 0xFF;
    out[index++] = (timestamp_ >> 2*8) & 0xFF;
    out[index++] = (timestamp_ >> 3*8) & 0xFF;
    out[index++] = (previousBlock_ >> 7*8) & 0xFF;
    out[index++] = (previousBlock_ >> 6*8) & 0xFF;
    out[index++] = (previousBlock_ >> 5*8) & 0xFF;
    out[index++] = (previousBlock_ >> 4*8) & 0xFF;
    out[index++] = (previousBlock_ >> 3*8) & 0xFF;
    out[index++] = (previousBlock_ >> 2*8) & 0xFF;
    out[index++] = (previousBlock_ >> 1*8) & 0xFF;
    out[index++] = (previousBlock_ >> 0*8) & 0xFF;
    out[index++] = (numberOfTransactions_ >> 0*8) & 0xFF;
    out[index++] = (numberOfTransactions_ >> 1*8) & 0xFF;
    out[index++] = (numberOfTransactions_ >> 2*8) & 0xFF;
    out[index++] = (numberOfTransactions_ >> 3*8) & 0xFF;
    out[index++] = (totalAmount_ >> 0*8) & 0xFF;
    out[index++] = (totalAmount_ >> 1*8) & 0xFF;
    out[index++] = (totalAmount_ >> 2*8) & 0xFF;
    out[index++] = (totalAmount_ >> 3*8) & 0xFF;
    out[index++] = (totalAmount_ >> 4*8) & 0xFF;
    out[index++] = (totalAmount_ >> 5*8) & 0xFF;
    out[index++] = (totalAmount_ >> 6*8) & 0xFF;
    out[index++] = (totalAmount_ >> 7*8) & 0xFF;
    out[index++] = (totalFee_ >> 0*8) & 0xFF;
    out[index++] = (totalFee_ >> 1*8) & 0xFF;
    out[index++] = (totalFee_ >> 2*8) & 0xFF;
    out[index++] = (totalFee_ >> 3*8) & 0xFF;
    out[index++] = (totalFee_ >> 4*8) & 0xFF;
    out[index++] = (totalFee_ >> 5*8) & 0xFF;
    out[index++] = (totalFee_ >> 6*8) & 0xFF;
    out[index++] = (totalFee_ >> 7*8) & 0xFF;
    out[index++] = (reward_ >> 0*8) & 0xFF;
    out[index++] = (reward_ >> 1*8) & 0xFF;
    out[index++] = (reward_ >> 2*8) & 0xFF;
    out[index++] = (reward_ >> 3*8) & 0xFF;
    out[index++] = (reward_ >> 4*8) & 0xFF;
    out[index++] = (reward_ >> 5*8) & 0xFF;
    out[index++] = (reward_ >> 6*8) & 0xFF;
    out[index++] = (reward_ >> 7*8) & 0xFF;
    out[index++] = (payloadLength_ >> 0*8) & 0xFF;
    out[index++] = (payloadLength_ >> 1*8) & 0xFF;
    out[index++] = (payloadLength_ >> 2*8) & 0xFF;
    out[index++] = (payloadLength_ >> 3*8) & 0xFF;
    for (int i = 0; i < payloadHash_.size(); ++i) {
        out[index++] = payloadHash_[i];
    }
    for (int i = 0; i < generatorPublicKey_.size(); ++i) {
        out[index++] = generatorPublicKey_[i];
    }

    assert(index == size);

    return out;
}

std::vector<unsigned char> BlockHeader::hash(std::vector<unsigned char> signature)
{
    auto out = std::vector<unsigned char>(crypto_hash_sha256_BYTES);

    auto message = serialize();
    message.insert(message.end(), signature.begin(), signature.end());
    crypto_hash_sha256(out.data(), message.data(), message.size());

    return out;
}

std::uint64_t BlockHeader::id(std::vector<unsigned char> signature)
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
