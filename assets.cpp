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

}

namespace Assets {

void peersEmpty(pqxx::read_transaction &db)
{
    {
        auto count = db.exec1("SELECT count(*) from peers")[0].as<int>();
        if (count != 0) throw std::runtime_error("Table peers not empty");
    }
    {
        auto count = db.exec1("SELECT count(*) from peers_dapp")[0].as<int>();
        if (count != 0) throw std::runtime_error("Table peers_dapp not empty");
    }
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

}
