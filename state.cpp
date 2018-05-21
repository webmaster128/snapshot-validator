#include "state.h"

#include <stdexcept>

#include "exceptions.h"

void validateState(const State state)
{
    for (auto &addressBalance : state.balances) {
        if (addressBalance.second < 0 && addressBalance.first != Exceptions::negativeBalanceAddress) {
            throw std::runtime_error(
                        "Negative balance for address " + std::to_string(addressBalance.first) +
                        ": " + std::to_string(addressBalance.second));
        }
    }
}
