#include "payload.h"

#include "sodium.h"

Payload::Payload(std::vector<TransactionRow> transactions)
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

std::vector<unsigned char> Payload::serialize() const
{
    std::vector<unsigned char> out;
    for (const auto &tws : transactions_) {
        auto serializedTransaction = tws.transaction.serialize();
        out.insert(out.end(), serializedTransaction.begin(), serializedTransaction.end());
        out.insert(out.end(), tws.signature.begin(), tws.signature.end());
        out.insert(out.end(), tws.secondSignature.begin(), tws.secondSignature.end());
    }
    return out;
}

std::vector<unsigned char> Payload::hash() const
{
    auto out = std::vector<unsigned char>(crypto_hash_sha256_BYTES);
    auto message = serialize();
    crypto_hash_sha256(out.data(), message.data(), message.size());
    return out;
}
