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
    float baseStockPrice;
    GenerateRandomMarket *market;
    MarketData *currentMarketData;

    Company(std::string companyName, std::string companyDescription, float baseStockPrice, float amplitude, float frequency, GraphDisplay* display);
    ~Company();

    void DisplayOnGraph(GenerateRandomMarket *market);
    void GenerateNext(GraphDisplay* display);

    float GetCurrentPrice() const;
    float CalculateIncrease(int startMonth, int startWeek, int endMonth, int endWeek);

    std::string GetName();
    std::string GetDescription();
private:
    std::vector<MarketData*> previousValues = std::vector<MarketData*>();
    int maximumAmountOfGraphData = 12; // 3 months
};



#endif //COMPANY_H
