#include "block.h"

#include <algorithm>
#include <assert.h>

#include <sodium.h>

bytes_t BlockHeader::serialize() const
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
    std::size_t index = 0;
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
    for (std::size_t i = 0; i < payloadHash.size(); ++i) {
        out[index++] = payloadHash[i];
    }
    for (std::size_t i = 0; i < generatorPublicKey.size(); ++i) {
        out[index++] = generatorPublicKey[i];
    }

    assert(index == size);

    return out;
}

bytes_t BlockHeader::hash(bytes_t signature) const
{
    auto out = std::vector<unsigned char>(crypto_hash_sha256_BYTES);

    auto message = serialize();
    message.insert(message.end(), signature.begin(), signature.end());
    crypto_hash_sha256(out.data(), message.data(), message.size());

    return out;
}

std::uint64_t BlockHeader::id(bytes_t signature) const
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
