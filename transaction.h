#pragma once

#include <cstdint>
#include <string>
#include <ostream>
#include <vector>

struct Transaction {
    Transaction(
        std::uint8_t type,
        std::uint32_t timestamp,
        std::vector<unsigned char> senderPublicKey,
        std::uint64_t recipientId,
        std::uint64_t amount,
        std::uint64_t fee,
        std::vector<unsigned char> assetData
    );

    const std::uint8_t type;
    const std::uint32_t timestamp;
    const std::vector<unsigned char> senderPublicKey;
    const std::uint64_t senderAddress;
    const std::uint64_t recipientAddress;
    const std::uint64_t amount;
    const std::uint64_t fee; // not signed
    const std::vector<unsigned char> assetData;

    std::vector<unsigned char> serialize() const;
    std::vector<unsigned char> hash(std::vector<unsigned char> signature = {}) const;
    std::uint64_t id(std::vector<unsigned char> signature) const;

    static std::vector<std::vector<unsigned char>> parseType4Pubkeys(const std::string transactionAsset);

    friend std::ostream& operator<<(std::ostream& os, const Transaction& transaction);
};

struct TransactionWithSignatures {
    TransactionWithSignatures(
            Transaction _transaction,
            std::vector<unsigned char> _signature,
            std::vector<unsigned char> _secondSignature)
        : transaction(_transaction)
        , signature(_signature)
        , secondSignature(_secondSignature)
    {
    }

    const Transaction transaction;
    const std::vector<unsigned char> signature;
    const std::vector<unsigned char> secondSignature;
};
