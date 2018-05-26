#pragma once

#include <pqxx/pqxx>

#include "blockchain_state.h"
#include "settings.h"

namespace Summaries {

void checkMemAccounts(pqxx::read_transaction &db, const BlockchainState &blockchainState, const Exceptions &exceptions);

}
