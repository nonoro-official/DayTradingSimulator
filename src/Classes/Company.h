//
// Created by sonik on 7/24/2025.
//

#ifndef COMPANY_H
#define COMPANY_H

#include "../Objects/GenerateRandomMarket.h"
#include <vector>

#include "MarketData.h"


class Company {
public:
    std::string companyName, companyDescription;
    float stockPrice;

    void UpdateStockPrice(float price);
private:
    std::vector<MarketData> previousValues;
};



#endif //COMPANY_H
