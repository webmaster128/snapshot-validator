#pragma once

#include <pqxx/pqxx>

namespace Assets {

void validateType0AssetData(pqxx::read_transaction &db);
void validateType1AssetData(pqxx::read_transaction &db);
void validateType2AssetData(pqxx::read_transaction &db);
void validateType3AssetData(pqxx::read_transaction &db);
void validateType4AssetData(pqxx::read_transaction &db);
void validateType5AssetData(pqxx::read_transaction &db);

}
