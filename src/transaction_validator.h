#pragma once

#include "settings.h"
#include "transaction.h"
#include "types.h"

namespace TransactionValidator {

void validate(const TransactionRow &row, const bytes_t &secondSignatureRequiredBy, const Exceptions &exceptions);

}
