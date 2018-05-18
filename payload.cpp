#include "payload.h"

#include "sodium.h"

Payload::Payload(std::vector<std::pair<Transaction, std::vector<unsigned char>>> transactions)
    : transactions_(transactions)
{
}

std::size_t Payload::transactionCount() const
{
    return transactions_.size();
}

std::vector<unsigned char> Payload::hash()
{
    auto out = std::vector<unsigned char>(crypto_hash_sha256_BYTES);
    crypto_hash_sha256_state state;
    crypto_hash_sha256_init(&state);
    for (const auto &transactionSignature : transactions_) {
        auto message = transactionSignature.first.serialize();
        message.insert(message.end(), transactionSignature.second.begin(), transactionSignature.second.end());
        crypto_hash_sha256_update(&state, message.data(), message.size());
    }
    crypto_hash_sha256_final(&state, out.data());
    return out;
}
