#pragma once

#include <pqxx/pqxx>

#include "blockchain_state.h"

namespace Summaries {

void checkMemAccounts(pqxx::read_transaction &db, const BlockchainState &blockchainState);

}
