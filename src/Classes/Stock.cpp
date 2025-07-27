//
// Created by sonik on 7/24/2025.
//

#include "Stock.h"

Stock::Stock(Company& c, float startingStockPrice)
{
    company = &c;
    shares = 1;
}


float Stock::GetShareValue() { return shares * company->GetCurrentPrice(); }