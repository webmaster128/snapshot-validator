#pragma once

#include "settings.h"
#include "transaction.h"

namespace TransactionValidator {

void validate(TransactionRow row, std::vector<unsigned char> secondSignatureRequiredBy, const Exceptions &exceptions);

}
