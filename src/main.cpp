#include <iostream>
#include "Market.h"
#include "OrderQueue.h"
#include "Player.h"
#include "Upgrades.h"
#include "Utils.h"
using namespace std;

int main()
{
    Market market;

    for (int i = 1; i <= 5; i++)
    {
        market.updatePrice();
        cout << "Market price: " << market.getCurrentPrice() << endl;
    }
    return 0;
}