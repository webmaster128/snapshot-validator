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
        std::vector<unsigned char> _assetData
        )
    : type(_type)
    , timestamp(_timestamp)
    , senderAddress(addressFromPubkey(_senderPublicKey))
    , senderPublicKey(_senderPublicKey)
    , recipientAddress(_recipientId)
    , amount(_amount)
    , fee(_fee)
    , assetData(_assetData)
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
