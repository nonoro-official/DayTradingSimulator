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
    std::string companyName, companyDescription, companyStoreDescription;
    float baseStockPrice;
    float minimumShares;
    GenerateRandomMarket *market = nullptr;
    MarketData *currentMarketData = nullptr;
    GraphDisplay* display;

    Company(std::string companyName, std::string companyDescription, float amplitude, float frequency, GraphDisplay* display);
    ~Company();

    void DisplayOnGraph(GenerateRandomMarket *market);
    void GenerateNext(GraphDisplay* display);

    float GetCurrentPrice() const;
    float CalculateIncreaseFromWeeksAgo(int weeksAgo);
    float CalculateAverageIncreaseOverWeeks(int totalWeeks);

    std::string GetName();
    std::string GetDescription();
    std::string GetStoreDescription();

    void SetStoreValues(std::string storeDescription, float basePrice);
    void SetMinimumShares(float minimumShares);
private:
    std::vector<MarketData*> previousValues = std::vector<MarketData*>();
    int maximumAmountOfGraphData = 12; // 3 months
};



#endif //COMPANY_H
