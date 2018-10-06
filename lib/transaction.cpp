#include "transaction.h"

#include <sodium.h>

#include "lisk.h"
#include "utils.h"

Transaction::Transaction(
        std::uint8_t _type,
        std::int32_t _timestamp,
        std::vector<unsigned char> _senderPublicKey,
        std::uint64_t _recipientId,
        std::uint64_t _amount,
        std::uint64_t _fee,
        std::vector<unsigned char> _assetData,
        std::uint64_t _dappId
        )
    : type(_type)
    , timestamp(_timestamp)
    , senderAddress(addressFromPubkey(_senderPublicKey))
    , senderPublicKey(_senderPublicKey)
    , recipientAddress(_recipientId)
    , amount(_amount)
    , fee(_fee)
    , assetData(_assetData)
    , dappId(_dappId)
    , type3Votes(_type == 3
                 ? Transaction::parseType3Votes(std::string(_assetData.begin(), _assetData.end()))
                 : VotesUpdate())
    , type4Pubkeys(_type == 4
                   ? Transaction::parseType4Pubkeys(std::string(_assetData.begin(), _assetData.end()))
                   : std::vector<std::vector<unsigned char>>())
{
}

std::vector<unsigned char> Transaction::serialize() const
{
    std::size_t size = 1 // type
            + 4 // timestamp
            + 32 // sender pubkey
            + 8 // recipient
            + 8 // amount
            + assetData.size()
            ;
    auto out = std::vector<unsigned char>(size);
    out[0] = type;
    out[1] = (timestamp >> 0) & 0xFF;
    out[2] = (timestamp >> 8) & 0xFF;
    out[3] = (timestamp >> 16) & 0xFF;
    out[4] = (timestamp >> 24) & 0xFF;
    for (int i = 0; i < senderPublicKey.size(); ++i) {
        out[5+i] = senderPublicKey[i];
    }

    for (int i = 0; i < 8; ++i) {
        out[37+i] = (recipientAddress >> (7-i)*8) & 0xFF;
    }

    for (int i = 0; i < 8; ++i) {
        out[45+i] = (amount >> i*8) & 0xFF;
    }

    for (int i = 0; i < assetData.size(); ++i) {
        out[53+i] = assetData[i];
    }

    return out;
}

std::vector<unsigned char> Transaction::hash(std::vector<unsigned char> signature, std::vector<unsigned char> secondSignature) const
{
    auto out = std::vector<unsigned char>(crypto_hash_sha256_BYTES);

    crypto_hash_sha256_state state;
    crypto_hash_sha256_init(&state);

    auto message = serialize();
    crypto_hash_sha256_update(&state, message.data(), message.size());
    crypto_hash_sha256_update(&state, signature.data(), signature.size());
    crypto_hash_sha256_update(&state, secondSignature.data(), secondSignature.size());

    crypto_hash_sha256_final(&state, out.data());

    return out;
}

std::uint64_t Transaction::id(std::vector<unsigned char> signature, std::vector<unsigned char> secondSignature) const
{
    return idFromEightBytes(firstEightBytesReversed(hash(signature, secondSignature)));
}

VotesUpdate Transaction::parseType3Votes(const std::string transactionAsset)
{
    VotesUpdate out;

    auto iterator = transactionAsset.cbegin();
    while (iterator != transactionAsset.cend()) {
        if (*iterator == ',') ++iterator;
        auto prefix = *iterator;
        ++iterator;
        auto pubkey = hex2Bytes(std::string(iterator, iterator+64));
        iterator += 64;
        if (prefix == '+') {
            out.added.push_back(pubkey);
        } else if (prefix == '-') {
            out.removed.push_back(pubkey);
        } else {
            throw std::runtime_error("Invalid prefix found in votes attset data: " + std::string(1, prefix));
        }
    }

    return out;
}

std::vector<std::vector<unsigned char>> Transaction::parseType4Pubkeys(const std::string transactionAsset)
{
    std::vector<std::vector<unsigned char>> out;

    auto iterator = transactionAsset.cbegin();
    iterator += 2; // skip min, lifetime
    while (iterator != transactionAsset.cend()) {
        if (*iterator == ',') ++iterator;
        ++iterator; // skip "+"
        auto pubkeyHex = std::string(iterator, iterator+64);
        out.push_back(hex2Bytes(pubkeyHex));
        iterator += 64;
    }

    return out;
}

std::ostream& operator<<(std::ostream& os, const Transaction& trx)
{
    os << std::to_string(trx.type)
       << "/" << trx.timestamp
       << "/" << trx.senderPublicKey.size()
       << "/" << trx.recipientAddress << "L"
       << "/" << trx.amount
    ;
    return os;
}
