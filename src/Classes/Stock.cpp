//
// Created by sonik on 7/24/2025.
//

#include "Stock.h"

#include "PlayerData.h"

Stock::Stock(Company& c, float minShares)
{
    company = &c;
    shares = 0;
    minimumShares = minShares;
}


float Stock::GetShareValue() { return shares * company->GetCurrentPrice(); }

float Stock::SellStock(float amountToSell) {
    float value = amountToSell * company->GetCurrentPrice();

    PlayerData::Instance().cash += value;

    return value;
}


void Stock::BuyStock(float depositAmount) {
    float rawShares = depositAmount / this->company->GetCurrentPrice();
    this->shares += std::round(rawShares * 100.0f) / 100.0f;  // Round to 2 decimal places
}