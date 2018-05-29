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

void validateReward(const BlockRow &row, const Settings &settings)
{
    auto actualReward = row.header.reward;

    std::uint64_t expectedReward;
    if (settings.exceptions.blockRewards.count(row.height))
    {
        expectedReward = settings.exceptions.blockRewards.at(row.height);
    }
    else if (row.height < settings.rewardOffset)
    {
        expectedReward = 0;
    }
    else if (row.height < settings.rewardOffset + 1*settings.rewardDistance)
    {
        expectedReward = 5 * 100000000;
    }
    else if (row.height < settings.rewardOffset + 2*settings.rewardDistance)
    {
        expectedReward = 4 * 100000000;
    }
    else if (row.height < settings.rewardOffset + 3*settings.rewardDistance)
    {
        expectedReward = 3 * 100000000;
    }
    else if (row.height < settings.rewardOffset + 4*settings.rewardDistance)
    {
        expectedReward = 2 * 100000000;
    }
    else
    {
        expectedReward = 1 * 100000000;
    }

    if (actualReward != expectedReward)
    {
        throw std::runtime_error("Block reward does not match the expected reward "
                                 "for block of height " + std::to_string(row.height) + "." +
                                 " Actual: " + std::to_string(actualReward) +
                                 " Expected: " + std::to_string(expectedReward)
                                 );
    }
}

}

namespace BlockValidator {

void validate(const BlockRow &row, const Settings &settings)
{
    validateId(row.header, row.id, row.signature);
    validateSignature(row.header, row.id, row.signature);
    validateReward(row, settings);
}

}
