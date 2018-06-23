#include "assets.h"

#include <iostream>

namespace {

void validateUniqueTransactionId(pqxx::read_transaction &db, const std::string tableName) {
    pqxx::result result = db.exec(
        "SELECT \"transactionId\" "
        "FROM " + tableName + R"SQL(
        GROUP BY "transactionId"
        HAVING count("transactionId") > 1
    )SQL", tableName);

    if (result.affected_rows() > 0) {
        for (auto row : result) {
            std::cout << "Transaction " << row[0].as<std::string>()
                      << " not unique in table " << tableName << std::endl;
        }
        throw std::runtime_error("Column transactionId not unique in table " + tableName);
    }
}

void checkUnconfirmed(pqxx::read_transaction &db, const std::string tableName, const std::string columnName)
{
    auto row = db.exec1("SELECT count(*) FROM " + tableName + " WHERE \"" + columnName + "\" != \"u_" + columnName + "\"");
    if (row[0].as<int>() != 0)
    {
        throw std::runtime_error("Column " + columnName + " does not match u_" + columnName + " in table " + tableName);
    }
}

}

namespace Assets {

void peersEmpty(pqxx::read_transaction &db)
{
    auto count = db.exec1("SELECT count(*) from peers")[0].as<int>();
    if (count != 0) throw std::runtime_error("Table peers not empty");
}

void peersDappEmpty(pqxx::read_transaction &db)
{
    auto count = db.exec1("SELECT count(*) from peers_dapp")[0].as<int>();
    if (count != 0) throw std::runtime_error("Table peers_dapp not empty");
}

void validateType0AssetData(pqxx::read_transaction &db)
{
    validateUniqueTransactionId(db, "transfer");
}

void validateType1AssetData(pqxx::read_transaction &db)
{
    validateUniqueTransactionId(db, "signatures");
}

void validateType2AssetData(pqxx::read_transaction &db)
{
    validateUniqueTransactionId(db, "delegates");
}

void validateType3AssetData(pqxx::read_transaction &db)
{
    validateUniqueTransactionId(db, "votes");
}

void validateType4AssetData(pqxx::read_transaction &db)
{
    validateUniqueTransactionId(db, "multisignatures");
}

void validateType5AssetData(pqxx::read_transaction &db)
{
    validateUniqueTransactionId(db, "dapps");
}

void validateType6AssetData(pqxx::read_transaction &db)
{
    validateUniqueTransactionId(db, "intransfer");
}

void validateType7AssetData(pqxx::read_transaction &db)
{
    validateUniqueTransactionId(db, "outtransfer");
}

void checkUnconfirmedInMemAccounts(pqxx::read_transaction &db)
{
    //checkUnconfirmed(db, "mem_accounts", "username");
    //checkUnconfirmed(db, "mem_accounts", "isDelegate");
    //checkUnconfirmed(db, "mem_accounts", "secondSignature");
    //checkUnconfirmed(db, "mem_accounts", "balance");
    //checkUnconfirmed(db, "mem_accounts", "delegates");
    //checkUnconfirmed(db, "mem_accounts", "multimin");
    //checkUnconfirmed(db, "mem_accounts", "multilifetime");
    //checkUnconfirmed(db, "mem_accounts", "nameexist");
}

}
