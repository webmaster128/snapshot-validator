#include "transactionvalidator.h"

#include <iostream>
#include <sodium.h>

#include "utils.h"

namespace TransactionValidator {

void validate(TransactionRow row, std::vector<unsigned char> secondSignatureRequiredBy, const Exceptions &exceptions)
{
    auto calculatedId = row.transaction.id(row.signature, row.secondSignature);
    if (row.id != calculatedId) {
        throw std::runtime_error("Transaction ID mismatch");
    }

    if (row.transaction.type != 0 && row.transaction.amount != 0 && exceptions.balanceAdjustments.count(row.id) == 0) {
        throw std::runtime_error(
                    "Amount not 0 for type " + std::to_string(row.transaction.type) +
                    " transaction " + std::to_string(row.id) + ": " + std::to_string(row.transaction.amount));
    }

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
