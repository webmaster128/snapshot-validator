#include "blockchain_state.h"

#include <stdexcept>

#include "lisk.h"

std::uint64_t AddressSummary::defaultLastBlockId = 0; // reset to genesis block in the main method

void BlockchainState::applyTransaction(const TransactionRow &transactionRow)
{
    const auto &t = transactionRow.transaction;

    switch(t.type) {
    case 0:
        addressSummaries[t.senderAddress].balance -= (t.amount + t.fee);
        addressSummaries[t.recipientAddress].balance += t.amount;
        addressSummaries[t.senderAddress].lastBlockId = transactionRow.blockId;
        addressSummaries[t.recipientAddress].lastBlockId = transactionRow.blockId;
        break;
    case 1:
        addressSummaries[t.senderAddress].balance -= t.fee;
        addressSummaries[t.senderAddress].secondPubkey = t.assetData;
        addressSummaries[t.senderAddress].lastBlockId = transactionRow.blockId;
        break;
    case 2:
        addressSummaries[t.senderAddress].balance -= t.fee;
        addressSummaries[t.senderAddress].delegateName = std::string(t.assetData.begin(), t.assetData.end());
        addressSummaries[t.senderAddress].lastBlockId = transactionRow.blockId;
        break;
    case 4: {
        addressSummaries[t.senderAddress].balance -= t.fee;
        addressSummaries[t.senderAddress].lastBlockId = transactionRow.blockId;

        for (auto &pubkey : t.type4Pubkeys) {
            // Ensure addresses from type 4 transactions exist
            (void) addressSummaries[addressFromPubkey(pubkey)];
        }
        break;
    }
    case 7:
        addressSummaries[t.senderAddress].balance -= (t.amount + t.fee);
        addressSummaries[t.recipientAddress].balance += t.amount;
        addressSummaries[t.senderAddress].lastBlockId = transactionRow.blockId;
        addressSummaries[t.recipientAddress].lastBlockId = transactionRow.blockId;
        break;
    default:
        addressSummaries[t.senderAddress].balance -= t.fee;
        addressSummaries[t.senderAddress].lastBlockId = transactionRow.blockId;
    }
}

void BlockchainState::applyBlock(const BlockHeader &bh, std::uint64_t blockId)
{
    addressSummaries[addressFromPubkey(bh.generatorPublicKey)].lastBlockId = blockId;
}
