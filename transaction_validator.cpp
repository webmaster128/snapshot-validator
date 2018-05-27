#include "transaction_validator.h"

#include <iostream>
#include <sodium.h>

#include "utils.h"

namespace {

void validate_id(const TransactionRow &row)
{
    auto calculatedId = row.transaction.id(row.signature, row.secondSignature);
    if (row.id != calculatedId) {
        throw std::runtime_error("Transaction ID mismatch");
    }
}

void validate_amount(const TransactionRow &row, const Exceptions &exceptions)
{
    if (exceptions.balanceAdjustments.count(row.id)) return;

    switch (row.transaction.type) {
    case 0:
    case 6:
    case 7:
        // any amount is okay
        break;
    case 1:
    case 2:
    case 3:
    case 4:
    case 5: {
        if (row.transaction.amount != 0) {
               throw std::runtime_error(
                           "Amount not 0 for type " + std::to_string(row.transaction.type) +
                           " transaction " + std::to_string(row.id) + ": " + std::to_string(row.transaction.amount));
        }
        break;
    }
    default:
        throw std::runtime_error("Unknown transaction type");
        break;
    }
}

void validate_fee(const TransactionRow &row, const Exceptions &exceptions)
{
    const auto t = row.transaction;
    const auto id = row.id;

    if (exceptions.transactionFees.count(id)) {
        auto expected = exceptions.transactionFees.at(id);
        if (t.fee != expected) {
            throw std::runtime_error("Transaction " + std::to_string(id) +
                                     " has different fee than expected by exception: " + std::to_string(t.fee));
        }
        return;
    }

    std::uint64_t expected;

    // https://github.com/LiskHQ/lisk-elements/blob/development/src/transactions/constants.js
    switch (t.type) {
    case 0:
        expected = t.assetData.empty() ? 10000000 : 20000000;
        break;
    case 1:
        expected = 500000000;
        break;
    case 2:
        expected = 2500000000;
        break;
    case 3:
        expected = 100000000;
        break;
    case 4:
        expected = 500000000 * (row.transaction.type4Pubkeys.size() + 1);
        break;
    case 5:
        expected = 2500000000;
        break;
    case 6:
        expected = 10000000;
        break;
    case 7:
        expected = 10000000;
        break;
    default:
        throw std::runtime_error("Unknown transaction type");
        break;
    }

    if (t.fee != expected) {
        throw std::runtime_error("Transaction " + std::to_string(id) + " type " + std::to_string(t.type) +
                                 " has invalid fee: " + std::to_string(t.fee) +
                                 " expected: "  + std::to_string(expected));
    }
}

void validate_signature(const TransactionRow &row, const std::vector<unsigned char> &secondSignatureRequiredBy)
{
    auto hash = row.transaction.hash();
    if (crypto_sign_verify_detached(row.signature.data(), hash.data(), hash.size(), row.transaction.senderPublicKey.data()) != 0) {
        std::cout << "ID: " << row.id << std::endl;
        std::cout << "Transaction: " << row.transaction << std::endl;
        std::cout << "Sender: " << bytes2Hex(row.transaction.senderPublicKey) << std::endl;
        std::cout << "Signature: " << bytes2Hex(row.signature) << std::endl;
        throw std::runtime_error("Invalid transaction signature");
    }

    if (!secondSignatureRequiredBy.empty()) {
        //std::cout << "Transaction: " << row.id << " requires second signature" << std::endl;
        auto hash2 = row.transaction.hash(row.signature);
        if (crypto_sign_verify_detached(row.secondSignature.data(), hash2.data(), hash2.size(), secondSignatureRequiredBy.data()) != 0) {
            std::cout << "ID: " << row.id << std::endl;
            std::cout << "Transaction: " << row.transaction << std::endl;
            std::cout << "Sender: " << bytes2Hex(row.transaction.senderPublicKey) << std::endl;
            std::cout << "Signature: " << bytes2Hex(row.signature) << std::endl;
            throw std::runtime_error("Invalid transaction second signature");
        }
    }
}

}

namespace TransactionValidator {

void validate(const TransactionRow &row, const std::vector<unsigned char> &secondSignatureRequiredBy, const Exceptions &exceptions)
{
    bool canBeSerialized = (exceptions.transactionsContainingInvalidRecipientAddress.count(row.id) == 0);
    if (canBeSerialized) {
        validate_id(row);
        validate_signature(row, secondSignatureRequiredBy);
    }

    validate_amount(row, exceptions);
    validate_fee(row, exceptions);
}

}
