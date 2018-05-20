#pragma once

#include <pqxx/pqxx>

#include "state.h"

namespace Summaries {

void checkMemAccounts(pqxx::read_transaction &db, const State &blockchainState);

}
