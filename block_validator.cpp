#include "block_validator.h"

#include <iostream>
#include <sodium.h>

#include "utils.h"

namespace {

void validateId(const BlockHeader &bh, std::uint64_t dbId, const bytes_t &signature)
{
    auto calculatedId = bh.id(signature);
    if (calculatedId != dbId)
    {
        throw std::runtime_error("Block ID mismatch");
    }
}

void validateSignature(const BlockHeader &bh, std::uint64_t dbId, const bytes_t &signature)
{
    auto hash = bh.hash();
    if (crypto_sign_verify_detached(signature.data(), hash.data(), hash.size(), bh.generatorPublicKey.data()) != 0) {
        std::cout << "Height: " << dbId << std::endl;
        std::cout << "Pubkey: " << bytes2Hex(bh.generatorPublicKey) << std::endl;
        std::cout << "Signature: " << bytes2Hex(signature) << std::endl;
        throw std::runtime_error("Invalid block signature");
    }
}


}

namespace BlockValidator {

void validate(const BlockRow &row)
{
    validateId(row.header, row.id, row.signature);
    validateSignature(row.header, row.id, row.signature);
}

}
