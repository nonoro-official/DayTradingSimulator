//
// Created by sonik on 7/24/2025.
//

#ifndef MARKETDATA_H
#define MARKETDATA_H

class Company;

struct MarketData {
    Company* company;
    float stockPrice;
    int monthAcquired;
    int weekAcquired;
};

#endif //MARKETDATA_H
