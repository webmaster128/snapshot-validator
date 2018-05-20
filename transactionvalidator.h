#pragma once

#include "transaction.h"

namespace TransactionValidator {

void validate(TransactionRow row, std::vector<unsigned char> secondSignatureRequiredBy);

}
