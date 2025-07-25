//
// Created by sonik on 7/24/2025.
//

#ifndef PLAYERDATA_H
#define PLAYERDATA_H
#include "Company.h"
#include "Stock.h"


class PlayerData {
public:
    float cash;
    int weekExecutionDelay;

    PlayerData();
private:
    std::vector<Stock> stocks = std::vector<Stock>();
};



#endif //PLAYERDATA_H
