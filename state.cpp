#include "state.h"

#include <stdexcept>

void validateState(const State state, const Settings &settings)
{
    for (auto &addressBalance : state.balances) {
        if (addressBalance.second < 0 && addressBalance.first != settings.negativeBalanceAddress) {
            throw std::runtime_error(
                        "Negative balance for address " + std::to_string(addressBalance.first) +
                        ": " + std::to_string(addressBalance.second));
        }
    }
}
