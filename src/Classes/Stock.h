//
// Created by sonik on 7/24/2025.
//

#ifndef STOCK_H
#define STOCK_H
#include "Company.h"


class Stock {
public:
    Company *company;
    float shares;
    float minimumShares;

    Stock(Company &c, float minShares);

    float GetShareValue();

    float SellStock(float shares);
    void BuyStock(float amountToBuy, float amountToSubtract);
};

#endif //STOCK_H
