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
        const unsigned char* senderPublicKeyBegin,
        std::size_t senderPublicKeyLength,
        std::uint64_t recipientId,
        std::uint64_t amount,
        const unsigned char* assetDataBegin,
        std::size_t assetDataLength
    );

    std::vector<unsigned char> serialize();
    std::vector<unsigned char> hash(std::vector<unsigned char> signature = {});
    std::uint64_t id(std::vector<unsigned char> signature);

    friend std::ostream& operator<<(std::ostream& os, const Transaction& transaction);

private:
    std::uint8_t type_;
    std::uint32_t timestamp_;
    const unsigned char* senderPublicKeyBegin_;
    std::size_t senderPublicKeyLength_;
    std::uint64_t recipientId_;
    std::uint64_t amount_;
    const unsigned char* assetDataBegin_;
    std::size_t assetDataLength_;
};
