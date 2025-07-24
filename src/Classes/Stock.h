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
};

#endif //STOCK_H
