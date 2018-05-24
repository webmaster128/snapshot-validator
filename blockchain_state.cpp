#include "blockchain_state.h"

#include <stdexcept>

#include "lisk.h"

void BlockchainState::applyTransaction(const Transaction &t)
{
    switch(t.type) {
    case 0:
        balances[t.senderAddress] -= (t.amount + t.fee);
        balances[t.recipientAddress] += t.amount;
        break;
    case 1:
        balances[t.senderAddress] -= t.fee;
        secondPubkeys[t.senderAddress] = t.assetData;
        break;
    case 2:
        balances[t.senderAddress] -= t.fee;
        delegateNames[t.senderAddress] = std::string(t.assetData.begin(), t.assetData.end());
        break;
    case 4: {
        balances[t.senderAddress] -= t.fee;

        // Ensure addresses from type 4 transactions exist
        for (auto &pubkey : t.type4Pubkeys) {
            balances[addressFromPubkey(pubkey)] += 0;
        }
        break;
    }
    default:
        balances[t.senderAddress] -= t.fee;
    }
}

void BlockchainState::validate(const Settings &settings) const
{
    for (auto &addressBalance : balances) {
        if (addressBalance.second < 0 && addressBalance.first != settings.negativeBalanceAddress) {
            throw std::runtime_error(
                        "Negative balance for address " + std::to_string(addressBalance.first) +
                        ": " + std::to_string(addressBalance.second));
        }
    }
}
