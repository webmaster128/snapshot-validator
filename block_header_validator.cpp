#include "block_header_validator.h"

#include <iostream>
#include <sodium.h>

#include "utils.h"

namespace BlockHeaderValidator {

void validate(const BlockHeader &bh, std::uint64_t dbId, const bytes_t &signature) {
    auto id = bh.id(signature);
    if (id != dbId) {
        throw std::runtime_error("id mismatch");
    }

    auto hash = bh.hash();
    if (crypto_sign_verify_detached(signature.data(), hash.data(), hash.size(), bh.generatorPublicKey.data()) != 0) {
        std::cout << "ID: " << dbId << std::endl;
        std::cout << "Pubkey: " << bytes2Hex(bh.generatorPublicKey) << std::endl;
        std::cout << "Signature: " << bytes2Hex(signature) << std::endl;
        throw std::runtime_error("Invalid block signature");
    }
}

}
