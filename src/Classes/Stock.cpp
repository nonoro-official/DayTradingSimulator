//
// Created by sonik on 7/24/2025.
//

#include "Stock.h"

Stock::Stock(Company& c, float minShares)
{
    company = &c;
    shares = 0;
    minimumShares = minShares;
}


float Stock::GetShareValue() { return shares * company->GetCurrentPrice(); }

float Stock::SellStock(float amountToSell) {
    float value = amountToSell * company->GetCurrentPrice();
    this->shares -= amountToSell;

    return value;
}

void Stock::BuyStock(float amountToBuy) {
    this->shares += amountToBuy;
}