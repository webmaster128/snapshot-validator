#include "transactionvalidator.h"

#include <iostream>
#include <sodium.h>

#include "utils.h"

namespace {

void validate_amount(const TransactionRow &row, const Exceptions &exceptions)
{
    if (exceptions.balanceAdjustments.count(row.id)) return;

    if (row.transaction.type != 0 && row.transaction.amount != 0) {
        throw std::runtime_error(
                    "Amount not 0 for type " + std::to_string(row.transaction.type) +
                    " transaction " + std::to_string(row.id) + ": " + std::to_string(row.transaction.amount));
    }
}

void validate_fee(const TransactionRow &row, const Exceptions &exceptions)
{
    const auto fee = row.transaction.fee;
    const auto id = row.id;

    if (exceptions.transactionFees.count(id)) {
        auto expected = exceptions.transactionFees.at(id);
        if (fee != expected) {
            throw std::runtime_error("Transaction " + std::to_string(id) +
                                     " has different fee than expected by exception: " + std::to_string(fee));
        }
        return;
    }

    // https://github.com/LiskHQ/lisk-elements/blob/development/src/transactions/constants.js
    switch (row.transaction.type) {
    case 0:
        if (fee != 10000000)
            throw std::runtime_error("Transaction " + std::to_string(id) + " type 0 fee invalid: " + std::to_string(fee));
        break;
    case 1:
        if (fee != 500000000)
            throw std::runtime_error("Transaction " + std::to_string(id) + " type 1 fee invalid: " + std::to_string(fee));
        break;
    case 2:
        if (fee != 2500000000)
            throw std::runtime_error("Transaction " + std::to_string(id) + " type 2 fee invalid: " + std::to_string(fee));
        break;
    case 3:
        if (fee != 100000000)
            throw std::runtime_error("Transaction " + std::to_string(id) + " type 3 fee invalid: " + std::to_string(fee));
        break;
    case 4:
        // TODO: fee is 5 LSK per key
        if (fee != 500000000)
            throw std::runtime_error("Transaction " + std::to_string(id) + " type 4 fee invalid: " + std::to_string(fee));
        break;
    case 5:
        if (fee != 2500000000)
            throw std::runtime_error("Transaction " + std::to_string(id) + " type 5 fee invalid: " + std::to_string(fee));
        break;
    case 6:
        if (fee != 100000000)
            throw std::runtime_error("Transaction " + std::to_string(id) + " type 6 fee invalid: " + std::to_string(fee));
        break;
    case 7:
        if (fee != 100000000)
            throw std::runtime_error("Transaction " + std::to_string(id) + " type 7 fee invalid: " + std::to_string(fee));
        break;
    default:
        break;
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
    auto calculatedId = row.transaction.id(row.signature, row.secondSignature);
    if (row.id != calculatedId) {
        throw std::runtime_error("Transaction ID mismatch");
    }

    validate_amount(row, exceptions);
    validate_fee(row, exceptions);
    validate_signature(row, secondSignatureRequiredBy);
}

}
