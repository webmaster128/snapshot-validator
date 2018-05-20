#include "transactionvalidator.h"

#include <iostream>
#include <sodium.h>

#include "utils.h"

namespace TransactionValidator {

void validate(Transaction t, std::vector<unsigned char> signature, std::vector<unsigned char> secondSignature)
{
    auto calculatedId = t.id(signature);

    //if (dbId != calculatedId) {
    //    throw std::runtime_error("Transaction ID mismatch");
    //}

    auto hash = t.hash();

    if (crypto_sign_verify_detached(signature.data(), hash.data(), hash.size(), t.senderPublicKey.data()) != 0) {
        std::cout << "ID: " << calculatedId << std::endl;
        std::cout << "Transaction: " << t << std::endl;
        std::cout << "Pubkey: " << bytes2Hex(t.senderPublicKey) << std::endl;
        std::cout << "Signature: " << bytes2Hex(signature) << std::endl;
        throw std::runtime_error("Invalid transaction signature");
    } else {
        // valid!
        //std::cout << "Transaction: " << t << std::endl;
        //std::cout << "Pubkey: " << bytes2Hex(std::vector<unsigned char>(senderPublicKeyBegin, senderPublicKeyBegin+senderPublicKeyLength)) << std::endl;
        //std::cout << "Valid ID: " << dbId << "/" << calculatedId << std::endl;
    }
}

}
