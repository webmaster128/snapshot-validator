#include "transactionvalidator.h"

#include <iostream>
#include <sodium.h>

#include "utils.h"

namespace TransactionValidator {

void validate(TransactionRow row, bool secondSignatureRequired)
{
    //auto calculatedId = row.transaction.id(row.signature);
    //if (row.id != calculatedId) {
    //    throw std::runtime_error("Transaction ID mismatch");
    //}

    auto hash = row.transaction.hash();

    if (crypto_sign_verify_detached(row.signature.data(), hash.data(), hash.size(), row.transaction.senderPublicKey.data()) != 0) {
        std::cout << "ID: " << row.id << std::endl;
        std::cout << "Transaction: " << row.transaction << std::endl;
        std::cout << "Sender: " << bytes2Hex(row.transaction.senderPublicKey) << std::endl;
        std::cout << "Signature: " << bytes2Hex(row.signature) << std::endl;
        throw std::runtime_error("Invalid transaction signature");
    } else {
        // valid!
        //std::cout << "Transaction: " << t << std::endl;
        //std::cout << "Pubkey: " << bytes2Hex(std::vector<unsigned char>(senderPublicKeyBegin, senderPublicKeyBegin+senderPublicKeyLength)) << std::endl;
        //std::cout << "Valid ID: " << dbId << "/" << calculatedId << std::endl;
    }

    if (secondSignatureRequired) {
        std::cout << "Transaction: " << row.id << " requires second signature" << std::endl;
    }
}

}
