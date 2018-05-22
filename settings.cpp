#include "settings.h"

Settings::Settings(Network network)
{
    switch (network) {
    case Network::Testnet:
        negativeBalanceAddress = 16009998050678037905ul;
        v100Compatible = false;
        exceptions.invalidTransactionSignature = {
            3274071402587084244ul,
            10403141873189588012ul,
            16896494584440078079ul,
        };
        break;
    case Network::Betanet:
        negativeBalanceAddress = 9594788837974552645ul;
        v100Compatible = true;
        break;
    }
}
