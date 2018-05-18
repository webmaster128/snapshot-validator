#include "state.h"

#include <stdexcept>

void validateState(const State state)
{
    for (auto &addressBalance : state.balances) {
        if (addressBalance.second < 0 && addressBalance.first != 9594788837974552645ul) {
            throw std::runtime_error(
                        "Negative balance for address " + std::to_string(addressBalance.first) +
                        ": " + std::to_string(addressBalance.second));
        }
    }
}
