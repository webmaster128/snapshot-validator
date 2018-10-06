#pragma once

#include <cstdint>
#include <vector>
#include <set>
#include <unordered_map>

using bytes_t = std::vector<unsigned char>;
using address_t = std::uint64_t;
using height_t = std::uint64_t;

struct Exceptions {
    std::uint64_t freeTransactionsBlockId; // i.e. genesis block
    std::unordered_map<std::uint64_t, bytes_t> transactionsSignedBy; // block id -> pubkey
    std::set<std::uint64_t> invalidTransactionSignature;
    std::set<std::uint64_t> inertTransactions;
    std::set<std::uint64_t> transactionsContainingInvalidRecipientAddress;
    std::set<std::string> invalidAddresses;
    std::set<std::uint64_t> payloadHashMismatch;
    std::unordered_map<std::uint64_t, std::int64_t> balanceAdjustments;
    std::unordered_map<height_t, std::uint64_t> blockRewards;
    std::unordered_map<std::uint64_t, int> rewardsFactor;
    std::unordered_map<std::uint64_t, int> feesFactor;
    std::unordered_map<std::uint64_t, int> feesBonus;
};

struct VotesUpdate {
    // lists of pubkeys
    std::vector<bytes_t> added;
    std::vector<bytes_t> removed;
};

template<typename type_of_key, typename type_of_value>
class tracking_unordered_map : public std::unordered_map<type_of_key, type_of_value>
{
public:
    type_of_value& operator[](const type_of_key& key)
    {
        dirtyKeys_.insert(key);
        return std::unordered_map<type_of_key, type_of_value>::operator[](key);
    }

    size_t erase(const type_of_key& key)
    {
        dirtyKeys_.erase(key);
        return std::unordered_map<type_of_key, type_of_value>::erase(key);
    }

    std::set<type_of_key> dirtyKeys() const
    {
        return dirtyKeys_;
    }

    void resetDirtyKeys()
    {
        dirtyKeys_.clear();
    }

private:
    std::set<type_of_key> dirtyKeys_;
};
