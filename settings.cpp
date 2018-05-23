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
        exceptions.inertTransactions = {
            16394286522174687330ul, // 1318685
            12298100805070303137ul, // 3057955
        };
        exceptions.recipientAddressOutOfRange = {
            393955899193580559ul, // 2511672
        };
        exceptions.transactionFees[16733264093386669800ul] = 0;
        exceptions.balanceAdjustments[15449731671927352923ul] = -1*100000000; // Burned 1 LSK as `amount` in a delegate vote
        break;
    case Network::Betanet:
        negativeBalanceAddress = 9594788837974552645ul;
        v100Compatible = true;
        break;
    }
}
