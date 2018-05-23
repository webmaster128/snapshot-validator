#include "settings.h"

Settings::Settings(Network network)
{
    switch (network) {
    case Network::Testnet:
        negativeBalanceAddress = 16009998050678037905ul;
        v100Compatible = false;
        exceptions.invalidTransactionSignature = {
            3274071402587084244ul,  // 595491
            10403141873189588012ul, // 624550
            16896494584440078079ul, // 631670
        };
        exceptions.balanceAdjustments[15449731671927352923ul] = -1*100000000; // Burned 1 LSK as `amount` in a delegate vote
        break;
    case Network::Betanet:
        negativeBalanceAddress = 9594788837974552645ul;
        v100Compatible = true;
        break;
    }
}
