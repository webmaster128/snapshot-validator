#pragma once

#include "blockheader.h"
#include "types.h"

namespace BlockHeaderValidator {
void validate(const BlockHeader &bh, uint64_t dbId, const bytes_t &signature);
}
