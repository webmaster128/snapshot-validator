#pragma once

#include "block.h"
#include "settings.h"
#include "types.h"

namespace BlockValidator {
void validate(const BlockRow &row, const Settings &settings);
}
