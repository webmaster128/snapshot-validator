#include "lisk.h"

#include <algorithm>
#include <sodium.h>

bytes_t firstEightBytesReversed(const bytes_t &data) {
    auto firstBytes = bytes_t{data.cbegin(), data.cbegin()+8};
    std::reverse(firstBytes.begin(), firstBytes.end());
    return firstBytes;
}

std::uint64_t idFromEightBytes(bytes_t firstBytes)
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

std::uint64_t addressFromPubkey(bytes_t publicKey) {
    //auto publicKeyHex = bytes2Hex(publicKey);

    auto hash = bytes_t(crypto_hash_sha256_BYTES);
    crypto_hash_sha256(hash.data(),
                       publicKey.data(),
                       publicKey.size());

    auto id = idFromEightBytes(firstEightBytesReversed(hash));

    return id;
}
