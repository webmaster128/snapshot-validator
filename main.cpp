#include <iostream>
#include <pqxx/pqxx>
#include <sodium.h>
#include <unordered_map>

#include "blockheader.h"
#include "lisk.h"
#include "payload.h"
#include "state.h"
#include "scopedbenchmark.h"
#include "summaries.h"
#include "transaction.h"
#include "transactionvalidator.h"
#include "utils.h"


int run(std::vector<std::string> args)
{
    ScopedBenchmark benchmarkFull("Overall runtime"); static_cast<void>(benchmarkFull);

    if (sodium_init() == -1) {
        return 1;
    }

    try
    {
        if (args.size() < 2) throw std::runtime_error("missing database name parameter");
        pqxx::connection dbConnection("dbname=" + args[1]);
        std::cout << "Connected to database " << dbConnection.dbname() << std::endl;
        pqxx::read_transaction db(dbConnection);

        {
            pqxx::result R = db.exec("SELECT COUNT(*) as number FROM trs");
            for (auto row : R) std::cout << "Transaction count " << row[0].c_str() << std::endl;
        }

        {
            pqxx::result R = db.exec("SELECT COUNT(*) as number FROM blocks");
            for (auto row : R) std::cout << "Blocks count " << row[0].c_str() << std::endl;
        }

        {
            pqxx::result R = db.exec("SELECT MAX(height) FROM blocks");
            for (auto row : R) std::cout << "Height: " << row[0].c_str() << std::endl;
        }


        std::unordered_map<std::uint64_t, std::vector<TransactionWithSignatures>> blockToTransactions;
        State blockchainState;

        {
            std::cout << "Reading transactions ..." << std::endl;
            ScopedBenchmark benchmarkTransactions("Reading transactions"); static_cast<void>(benchmarkTransactions);

            pqxx::result R = db.exec(R"SQL(
                SELECT
                    id, "blockId", trs.type, timestamp, "senderPublicKey", coalesce(left("recipientId", -1), '0') AS recipient_address,
                    amount, fee, signature, "signSignature",
                    transfer.data AS type0_asset,
                    signatures."publicKey" AS type1_asset,
                    delegates.username AS type2_asset,
                    replace(votes.votes, ',', '') AS type3_asset,
                    coalesce(multisignatures.min, 0) AS type4_asset_min, coalesce(multisignatures.lifetime, 0) AS type4_asset_lifetime,
                    replace(multisignatures.keysgroup, ',', '') AS type4_asset_keys,
                    (coalesce(dapps.name, '') || coalesce(dapps.description, '') || coalesce(dapps.tags, '') || coalesce(dapps.link, '') || coalesce(dapps.icon, '')) AS type5_asset_texts,
                    coalesce(dapps.type, 0) AS type5_asset_type, coalesce(dapps.category, 0) AS type5_asset_category
                FROM trs
                LEFT JOIN transfer ON trs.id = transfer."transactionId"
                LEFT JOIN signatures ON trs.id = signatures."transactionId"
                LEFT JOIN delegates ON trs.id = delegates."transactionId"
                LEFT JOIN votes ON trs.id = votes."transactionId"
                LEFT JOIN multisignatures ON trs.id = multisignatures."transactionId"
                LEFT JOIN dapps ON trs.id = dapps."transactionId"
                ORDER BY "rowId"
            )SQL");
            for (auto row : R) {
                // Read fields in row
                int index = 0;
                auto dbId = row[index++].as<std::uint64_t>();
                auto dbBockId = row[index++].as<std::uint64_t>();
                auto dbType = row[index++].as<int>();
                auto dbTimestamp = row[index++].as<std::uint32_t>();
                auto dbSenderPublicKey = pqxx::binarystring(row[index++]);
                auto dbRecipientId = row[index++].as<std::uint64_t>();
                auto dbAmount = row[index++].as<std::uint64_t>();
                auto dbFee = row[index++].as<std::uint64_t>();
                auto dbSignature = pqxx::binarystring(row[index++]);
                auto dbSecondSignature = pqxx::binarystring(row[index++]);
                auto dbType0Asset = pqxx::binarystring(row[index++]);
                auto dbType1Asset = pqxx::binarystring(row[index++]);
                auto dbType2Asset = row[index++].get<std::string>();
                auto dbType3Asset = row[index++].get<std::string>();
                auto dbType4AssetMin = row[index++].as<int>();
                auto dbType4AssetLifetime = row[index++].as<int>();
                auto dbType4AssetKeys = row[index++].get<std::string>();
                auto dbType5AssetText = row[index++].get<std::string>();
                auto dbType5AssetType = row[index++].as<std::uint32_t>();
                auto dbType5AssetCategory = row[index++].as<std::uint32_t>();

                // Parse fields in row
                auto senderPublicKey = asVector(dbSenderPublicKey);
                auto signature = asVector(dbSignature);
                auto secondSignature = asVector(dbSecondSignature);

                std::vector<unsigned char> assetData = {};
                switch (dbType) {
                case 0:
                    assetData = asVector(dbType0Asset);
                    break;
                case 1:
                    assetData = asVector(dbType1Asset);
                    break;
                case 2:
                    if (dbType2Asset) {
                        assetData = asVector(*dbType2Asset);
                    }
                    break;
                case 3:
                    assetData = asVector(*dbType3Asset);
                    break;
                case 4: {
                    assetData.push_back(static_cast<std::uint8_t>(dbType4AssetMin));
                    assetData.push_back(static_cast<std::uint8_t>(dbType4AssetLifetime));
                    auto keys = asVector(*dbType4AssetKeys);
                    assetData.insert(assetData.end(), keys.cbegin(), keys.cend());
                    break;
                }
                case 5:
                    assetData = asVector(*dbType5AssetText);
                    assetData.push_back((dbType5AssetType >> 0*8) & 0xff);
                    assetData.push_back((dbType5AssetType >> 1*8) & 0xff);
                    assetData.push_back((dbType5AssetType >> 2*8) & 0xff);
                    assetData.push_back((dbType5AssetType >> 3*8) & 0xff);
                    assetData.push_back((dbType5AssetCategory >> 0*8) & 0xff);
                    assetData.push_back((dbType5AssetCategory >> 1*8) & 0xff);
                    assetData.push_back((dbType5AssetCategory >> 2*8) & 0xff);
                    assetData.push_back((dbType5AssetCategory >> 3*8) & 0xff);
                    break;
                }

                auto t = Transaction(
                    dbType,
                    dbTimestamp,
                    senderPublicKey,
                    dbRecipientId,
                    dbAmount,
                    dbFee,
                    assetData
                );
                blockToTransactions[dbBockId].emplace_back(t, signature, secondSignature);
            }
        }


        {
            std::cout << "Reading blocks ..." << std::endl;
            ScopedBenchmark benchmarkBlocks("Reading blocks"); static_cast<void>(benchmarkBlocks);

            pqxx::result R = db.exec(R"SQL(
                SELECT
                    id, version, timestamp, height, "previousBlock", "numberOfTransactions", "totalAmount", "totalFee", reward,
                    "payloadLength", "payloadHash", "generatorPublicKey", "blockSignature"
                FROM blocks
                ORDER BY height
            )SQL");

            std::uint64_t lastHeight = 0;
            std::uint64_t lastBlockId = 0;
            std::uint64_t roundFees = 0;
            std::vector<std::uint64_t> roundDelegates = std::vector<std::uint64_t>(101);
            std::vector<std::uint64_t> roundRewards = std::vector<std::uint64_t>(101);
            for (auto row : R) {
                int index = 0;
                const auto dbId = row[index++].as<std::uint64_t>();
                const auto dbVersion = row[index++].as<std::uint32_t>();
                const auto dbTimestamp = row[index++].as<std::uint32_t>();
                const auto dbHeight = row[index++].as<std::uint64_t>();
                const auto dbPreviousBlock = row[index++].get<std::uint64_t>();
                const auto dbNumberOfTransactions = row[index++].as<std::uint32_t>();
                const auto dbTotalAmount = row[index++].as<std::uint64_t>();
                const auto dbTotalFee = row[index++].as<std::uint64_t>();
                const auto dbReward = row[index++].as<std::uint64_t>();
                const auto dbPayloadLength = row[index++].as<std::uint32_t>();
                const auto dbPayloadHash = pqxx::binarystring(row[index++]);
                const auto dbGeneratorPublicKey = pqxx::binarystring(row[index++]);
                const auto dbSignature = pqxx::binarystring(row[index++]);

                const auto generatorPublicKey = asVector(dbGeneratorPublicKey);
                const auto payloadHash = asVector(dbPayloadHash);
                const auto signature = asVector(dbSignature);

                if (dbHeight != lastHeight + 1) {
                    throw std::runtime_error("Height mismatch");
                }
                lastHeight = dbHeight;

                if (dbHeight != 1) {
                    if (*dbPreviousBlock != lastBlockId) {
                        throw std::runtime_error("previous block mismatch");
                    }
                }
                lastBlockId = dbId;


                BlockHeader bh(
                    dbVersion,
                    dbTimestamp,
                    dbPreviousBlock ? *dbPreviousBlock : 0,
                    dbNumberOfTransactions,
                    dbTotalAmount,
                    dbTotalFee,
                    dbReward,
                    dbPayloadLength,
                    payloadHash,
                    generatorPublicKey
                );

                auto hash = bh.hash();
                if (crypto_sign_verify_detached(signature.data(), hash.data(), hash.size(), generatorPublicKey.data()) != 0) {
                    std::cout << "ID: " << dbId << std::endl;
                    std::cout << "Pubkey: " << bytes2Hex(generatorPublicKey) << std::endl;
                    std::cout << "Signature: " << bytes2Hex(signature) << std::endl;
                    throw std::runtime_error("Invalid block signature");
                }

                Payload payload(blockToTransactions[dbId]);
                if (payload.transactionCount() != dbNumberOfTransactions) {
                    throw std::runtime_error("transaction number mismatch");
                }

                auto id = bh.id(signature);
                if (id != dbId) {
                    throw std::runtime_error("id mismatch");
                }

                auto calculatedPayloadHash = payload.hash();
                if (payloadHash != calculatedPayloadHash) {
                    auto payloadSerialized = payload.serialize();
                    std::cout << "payload length calculated: " << payloadSerialized.size()
                              << " expected: " << bh.payloadLength << std::endl;
                    // std::cout << "payload: " << bytes2Hex(payloadSerialized) << std::endl;

                    for (auto &tws : blockToTransactions[id]) {
                        auto transactionId = tws.transaction.id(tws.signature);
                        std::cout << tws.transaction << " " << transactionId << std::endl;
                    }

                    if (dbHeight == 1) {
                        // warn only (https://github.com/LiskHQ/lisk/issues/2047)
                        std::cout << "payload hash mismatch for block " << id << std::endl;
                    } else {
                        throw std::runtime_error("payload hash mismatch");
                    }
                }

                for (auto &tws : blockToTransactions[dbId]) {
                    auto &t = tws.transaction;

                    // Validate transaction

                    if (t.type == 0 || (t.type == 2 && t.timestamp != 0)) {
                        TransactionValidator::validate(t, tws.signature, tws.secondSignature);
                    } else {
                        std::cout << "Transaction not verified: " << t << std::endl;
                    }

                    // Update state from transaction

                    switch(t.type) {
                    case 0:
                        blockchainState.balances[t.senderAddress] -= (t.amount + t.fee);
                        blockchainState.balances[t.recipientAddress] += t.amount;
                        break;
                    case 1:
                        blockchainState.balances[t.senderAddress] -= t.fee;
                        blockchainState.secondPubkeys[t.senderAddress] = t.assetData;
                        break;
                    case 2:
                        blockchainState.balances[t.senderAddress] -= t.fee;
                        blockchainState.delegateNames[t.senderAddress] = std::string(t.assetData.begin(), t.assetData.end());
                        break;
                    case 4: {
                        blockchainState.balances[t.senderAddress] -= t.fee;

                        // Ensure addresses from type 4 transactions exist
                        auto pubkeys = Transaction::parseType4Pubkeys(std::string(t.assetData.begin(), t.assetData.end()));
                        for (auto &pubkey : pubkeys) {
                            blockchainState.balances[addressFromPubkey(pubkey)] += 0;
                        }
                        break;
                    }
                    default:
                        blockchainState.balances[t.senderAddress] -= t.fee;
                    }

                    validateState(blockchainState);
                }

                roundFees += bh.totalFee;
                roundDelegates[(dbHeight-1)%101] = addressFromPubkey(bh.generatorPublicKey);
                roundRewards[(dbHeight-1)%101] = bh.reward;

                bool isLast = (dbHeight%101 == 0);
                //std::cout << "Block: " << id << " in round " << roundFromHeight(dbHeight) << " last: " << isLast << " reward: " << bh.reward << std::endl;

                if (isLast) {
                    auto feePerDelegate = roundFees/101;
                    auto feeRemaining = roundFees - (101*feePerDelegate);

                    for (int i = 0; i < 101; ++i) {
                        auto reward = roundRewards[i];
                        blockchainState.balances[roundDelegates[i]] += (reward + feePerDelegate);
                    }

                    if (feeRemaining > 0) {
                        // rest goes to the last delegate
                        blockchainState.balances[roundDelegates[100]] += feeRemaining;
                    }

                    roundFees = 0;
                }

            }
        }

        // validate after all blocks
        validateState(blockchainState);

        Summaries::checkMemAccounts(db, blockchainState);

        std::cout << "lisksnake balance: " << blockchainState.balances[14272331929440866024ul] << std::endl;
        std::cout << "prolina balance: " << blockchainState.balances[2178850910632340753ul] << std::endl;

        db.commit();
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    return 0;
}

int main(int argc, char* argv[]) {
    std::vector<std::string> args(argv, argv + argc);
    return run(args);
}
