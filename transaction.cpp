#include "transaction.h"

#include <algorithm>
#include <sodium.h>

Transaction::Transaction(//std::uint64_t id,
        std::uint8_t type,
        std::uint32_t timestamp,
        const unsigned char* senderPublicKeyBegin,
        std::size_t senderPublicKeyLength,
        std::uint64_t recipientId,
        std::uint64_t amount,
        const unsigned char* assetDataBegin,
        std::size_t assetDataLength
        )
    : type_(type)
    , timestamp_(timestamp)
    , senderPublicKeyBegin_(senderPublicKeyBegin)
    , senderPublicKeyLength_(senderPublicKeyLength)
    , recipientId_(recipientId)
    , amount_(amount)
    , assetDataBegin_(assetDataBegin)
    , assetDataLength_(assetDataLength)
{
}

std::vector<unsigned char> Transaction::serialize()
{
    std::size_t size = 1 // type
            + 4 // timestamp
            + 32 // sender pubkey
            + 8 // recipient
            + 8 // amount
            + assetDataLength_
            ;
    auto out = std::vector<unsigned char>(size);
    out[0] = type_;
    out[1] = (timestamp_ >> 0) & 0xFF;
    out[2] = (timestamp_ >> 8) & 0xFF;
    out[3] = (timestamp_ >> 16) & 0xFF;
    out[4] = (timestamp_ >> 24) & 0xFF;
    for (int i = 0; i < senderPublicKeyLength_; ++i) {
        out[5+i] = *(senderPublicKeyBegin_ + i);
    }

    for (int i = 0; i < 8; ++i) {
        out[37+i] = (recipientId_ >> (7-i)*8) & 0xFF;
    }

    for (int i = 0; i < 8; ++i) {
        out[45+i] = (amount_ >> i*8) & 0xFF;
    }

    for (int i = 0; i < assetDataLength_; ++i) {
        out[53+i] = *(assetDataBegin_ + i);
    }

    return out;
}

std::vector<unsigned char> Transaction::hash(std::vector<unsigned char> signature)
{
    auto out = std::vector<unsigned char>(crypto_hash_sha256_BYTES);

    auto message = serialize();
    message.insert(message.end(), signature.begin(), signature.end());
    crypto_hash_sha256(out.data(), message.data(), message.size());

    return out;
}

std::uint64_t Transaction::id(std::vector<unsigned char> signature)
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

std::ostream& operator<<(std::ostream& os, const Transaction& trx)
{
    os << std::to_string(trx.type_)
       << "/" << trx.timestamp_
       << "/" << trx.senderPublicKeyLength_
       << "/" << trx.recipientId_ << "L"
       << "/" << trx.amount_
    ;
    return os;
}
