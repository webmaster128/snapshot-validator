#include "payload.h"

#include "sodium.h"

Payload::Payload(std::vector<TransactionWithSignatures> transactions)
    : transactions_(transactions)
{
}

std::size_t Payload::transactionCount() const
{
    return transactions_.size();
}

std::vector<Transaction> Payload::transactions() const
{
    auto out = std::vector<Transaction>();
    out.reserve(transactionCount());

    for (const auto &tws : transactions_) {
        out.push_back(tws.transaction);
    }

    return out;
}

std::vector<unsigned char> Payload::hash()
{
    auto out = std::vector<unsigned char>(crypto_hash_sha256_BYTES);
    crypto_hash_sha256_state state;
    crypto_hash_sha256_init(&state);
    for (const auto &tws : transactions_) {
        auto message = tws.transaction.serialize();
        message.insert(message.end(), tws.signature.begin(), tws.signature.end());
        message.insert(message.end(), tws.secondSignature.begin(), tws.secondSignature.end());
        crypto_hash_sha256_update(&state, message.data(), message.size());
    }
    crypto_hash_sha256_final(&state, out.data());
    return out;
}
