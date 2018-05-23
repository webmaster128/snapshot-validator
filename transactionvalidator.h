#pragma once

#include "settings.h"
#include "transaction.h"

namespace TransactionValidator {

void validate(const TransactionRow &row, const std::vector<unsigned char> &secondSignatureRequiredBy, const Exceptions &exceptions);

}
