#include "blockchain_state.h"

#include <stdexcept>

#include "lisk.h"

void BlockchainState::applyTransaction(const TransactionRow &transactionRow)
{
    const auto &t = transactionRow.transaction;

    switch(t.type) {
    case 0:
        balances[t.senderAddress] -= (t.amount + t.fee);
        balances[t.recipientAddress] += t.amount;
        lastBlockId[t.senderAddress] = transactionRow.blockId;
        lastBlockId[t.recipientAddress] = transactionRow.blockId;
        break;
    case 1:
        balances[t.senderAddress] -= t.fee;
        secondPubkeys[t.senderAddress] = t.assetData;
        lastBlockId[t.senderAddress] = transactionRow.blockId;
        break;
    case 2:
        balances[t.senderAddress] -= t.fee;
        delegateNames[t.senderAddress] = std::string(t.assetData.begin(), t.assetData.end());
        lastBlockId[t.senderAddress] = transactionRow.blockId;
        break;
    case 4: {
        balances[t.senderAddress] -= t.fee;

        // Ensure addresses from type 4 transactions exist
        for (auto &pubkey : t.type4Pubkeys) {
            balances[addressFromPubkey(pubkey)] += 0;
        }
        lastBlockId[t.senderAddress] = transactionRow.blockId;
        break;
    }
    case 7:
        balances[t.senderAddress] -= (t.amount + t.fee);
        balances[t.recipientAddress] += t.amount;
        lastBlockId[t.senderAddress] = transactionRow.blockId;
        lastBlockId[t.recipientAddress] = transactionRow.blockId;
        break;
    default:
        balances[t.senderAddress] -= t.fee;
        lastBlockId[t.senderAddress] = transactionRow.blockId;
    }
}

void BlockchainState::applyBlock(const BlockHeader &bh, std::uint64_t blockId)
{
    lastBlockId[addressFromPubkey(bh.generatorPublicKey)] = blockId;
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
