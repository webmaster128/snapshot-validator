#pragma once

#include <cstdint>
#include <string>
#include <ostream>
#include <vector>

class Transaction {

public:
    Transaction(
        std::uint8_t type,
        std::uint32_t timestamp,
        std::vector<unsigned char> senderPublicKey,
        std::uint64_t recipientId,
        std::uint64_t amount,
        const unsigned char* assetDataBegin,
        std::size_t assetDataLength
    );

    const std::uint8_t type;
    const std::uint64_t senderAddress;
    const std::uint64_t recipientAddress;
    const std::uint64_t amount;

    std::vector<unsigned char> serialize() const;
    std::vector<unsigned char> hash(std::vector<unsigned char> signature = {}) const;
    std::uint64_t id(std::vector<unsigned char> signature) const;

    friend std::ostream& operator<<(std::ostream& os, const Transaction& transaction);

private:
    std::uint32_t timestamp_;
    std::vector<unsigned char> senderPublicKey_;
    const unsigned char* assetDataBegin_;
    std::size_t assetDataLength_;
};
