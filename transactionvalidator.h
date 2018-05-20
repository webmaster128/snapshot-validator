#pragma once

#include "transaction.h"

namespace TransactionValidator {

void validate(Transaction t, std::vector<unsigned char> signature, std::vector<unsigned char> secondSignature);

}
