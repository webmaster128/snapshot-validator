#pragma once

#include "transaction.h"

class Payload {

public:
    Payload(
            std::vector<TransactionWithSignatures> transactions
    );

    std::size_t transactionCount() const;
    std::vector<Transaction> transactions() const;

    std::vector<unsigned char> serialize() const;
    std::vector<unsigned char> hash() const;

private:
    std::vector<TransactionWithSignatures> transactions_;
};
