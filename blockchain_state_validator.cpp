#include "blockchain_state_validator.h"


namespace BlockchainStateValidator {

void validate(const BlockchainState &state, const Settings &settings)
{
    for (const auto &keyValue : state.addressSummaries) {
        const auto address = keyValue.first;
        const auto summary = keyValue.second;
        if (summary.balance < 0 && address != settings.negativeBalanceAddress) {
            throw std::runtime_error(
                        "Negative balance for address " + std::to_string(address) +
                        ": " + std::to_string(summary.balance));
        }
    }
}

}
