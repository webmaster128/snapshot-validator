#include "settings.h"

Settings::Settings(Network network)
{
    switch (network) {
    case Network::Testnet:
        negativeBalanceAddress = 16009998050678037905ul;
        v100Compatible = false;
        break;
    case Network::Betanet:
        negativeBalanceAddress = 9594788837974552645ul;
        v100Compatible = true;
        break;
    }
}
