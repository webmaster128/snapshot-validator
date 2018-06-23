#include "blockchain_state_validator.h"


namespace BlockchainStateValidator {

void validate(BlockchainState &state, const Settings &settings)
{
    for (const auto &address : state.addressSummaries.dirtyKeys())
    {
        const auto summary = state.addressSummaries.at(address);

        if (summary.balance < 0 && address != settings.negativeBalanceAddress) {
            throw std::runtime_error(
                        "Negative balance for address " + std::to_string(address) +
                        ": " + std::to_string(summary.balance));
        }
    }

    state.addressSummaries.resetDirtyKeys();
}

}
