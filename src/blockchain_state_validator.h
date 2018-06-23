#pragma once

#include "blockchain_state.h"
#include "settings.h"

namespace BlockchainStateValidator {

void validate(BlockchainState &state, const Settings &settings);

}
