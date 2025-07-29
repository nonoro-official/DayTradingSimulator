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

    Stock(Company &c, float startingStockPrice);

    float GetShareValue();

    float SellStock(float shares);
    void BuyStock(float shares);
};

#endif //STOCK_H
