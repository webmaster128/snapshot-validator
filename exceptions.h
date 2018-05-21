#pragma once

#define TESTNET 1
//#define BETANET 1

#include <cstdint>

struct Exceptions {

#if defined(TESTNET)
    static const std::uint64_t negativeBalanceAddress = 16009998050678037905ul;
#elif defined(BETANET)
    static const std::uint64_t negativeBalanceAddress = 9594788837974552645ul;
#endif

};
