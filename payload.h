#pragma once

#include "transaction.h"

class Payload {

public:
    Payload(
            std::vector<std::pair<Transaction, std::vector<unsigned char>>> transactions
    );

    std::size_t transactionCount() const;
    std::vector<Transaction> transactions() const;

    std::vector<unsigned char> hash();

private:
    std::vector<std::pair<Transaction, std::vector<unsigned char>>> transactions_;
};
