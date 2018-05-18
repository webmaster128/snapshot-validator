#include "lisk.h"

#include <algorithm>
#include <sodium.h>

std::vector<unsigned char> firstEightBytesReversed(const std::vector<unsigned char> &data) {
    auto firstBytes = std::vector<unsigned char>{data.cbegin(), data.cbegin()+8};
    std::reverse(firstBytes.begin(), firstBytes.end());
    return firstBytes;
}

std::uint64_t idFromEightBytes(std::vector<unsigned char> firstBytes)
{
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

std::uint64_t addressFromPubkey(std::vector<unsigned char> publicKey) {
    //auto publicKeyHex = bytes2Hex(publicKey);

    auto hash = std::vector<unsigned char>(crypto_hash_sha256_BYTES);
    crypto_hash_sha256(hash.data(),
                       publicKey.data(),
                       publicKey.size());

    auto id = idFromEightBytes(firstEightBytesReversed(hash));

    return id;
}
