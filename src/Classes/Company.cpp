//
// Created by sonik on 7/24/2025.
//

#include "Company.h"

#include <iostream>

std::pair<int, int> GetPastDate(int currentMonth, int currentWeek, int weeksAgo) {
    int totalWeeks = currentMonth * 4 + currentWeek;
    int pastTotal = totalWeeks - weeksAgo;

    if (pastTotal < 0) pastTotal = 0; // Clamp to zero to avoid negative

    int pastMonth = pastTotal / 4;
    int pastWeek = pastTotal % 4;

    return {pastMonth, pastWeek};
}

Company::Company(std::string companyName, std::string companyDescription, float amplitude, float frequency, GraphDisplay* display) {
    this->companyName = companyName;
    this->companyDescription = companyDescription;

    // initialize market
    market = new GenerateRandomMarket(amplitude, frequency);
    market->InitializeMarket();

    // Add Listener
    this->display = display;
    GameState::Instance().AddTickListener([this]() {
        GenerateNext(this->display);

        /*std::cout << "CurrentPrice: " << this->GetCurrentPrice() << " Month: " << currentMarketData->monthAcquired << " Week: " << currentMarketData->weekAcquired << std::endl;
        std::cout << "Increase: " << CalculateIncrease(0, 0, GameState::Instance().GetMonth(), GameState::Instance().GetWeek()) << "%" << std::endl;
    */
    });
}

void Company::SetStoreValues(std::string storeDescription, float basePrice) {
    this->companyStoreDescription = storeDescription;
    this->baseStockPrice = basePrice;

    // pre gen points
    std::vector<GraphPoint*> history = market->GeneratePrefillPoints(25);

    int totalPoints = static_cast<int>(history.size());
    int weeksBack = totalPoints;

    float lastMultiplier = 0;
    for (int i = 0; i < totalPoints; ++i) {
        GraphPoint* point = history[i];

        int pastTotalWeeks = -weeksBack + i + 1;
        int month = pastTotalWeeks / 4;
        int week = pastTotalWeeks % 4;

        if (week < 1) {
            week += 4;
            month -= 1;
        }

        MarketData* data = new MarketData();
        data->company = this;
        data->monthAcquired = month;
        data->weekAcquired = week;

        float multiplier = point->yValue;
        data->stockPrice = baseStockPrice * multiplier;

        lastMultiplier = multiplier;

        float increase = 0.0f;
        if (!previousValues.empty()) {
            increase = data->stockPrice - previousValues.back()->stockPrice;
        }

        point->InitializeTooltip(week, month, data->stockPrice, increase);
        previousValues.push_back(data);
    }

    // === SET CURRENT MARKET DATA ===
    if (!previousValues.empty()) {
        currentMarketData = previousValues.back();
        market->SetCurrentValue(lastMultiplier);
    }

    display->PrefillPoints(history);
}


void Company::GenerateNext(GraphDisplay* display) {
    // Generate point
    GraphPoint* nextPoint = market->GenerateNextPoint();

    // Create data
    MarketData* newData = new MarketData();
    newData->company = this;
    newData->monthAcquired = GameState::Instance().GetMonth();
    newData->weekAcquired = GameState::Instance().GetWeek();

    float multiplier = nextPoint->yValue;
    newData->stockPrice = this->baseStockPrice * multiplier;
    
    // Pass tooltip data
    float increase = 0;
    if (!previousValues.empty()) {
        increase = newData->stockPrice - previousValues.back()->stockPrice;
    }
    nextPoint->InitializeTooltip(newData->weekAcquired, newData->monthAcquired, newData->stockPrice, increase);

    // Save data
    this->currentMarketData = newData;

    // Backlog
    if (this->previousValues.size() >= maximumAmountOfGraphData) {
        // Free memory of the oldest item
        delete this->previousValues.front();
        // Remove pointer from the vector
        this->previousValues.erase(this->previousValues.begin());
    }

    this->previousValues.push_back(newData);

    // Show on Graph
    display->AddNode(nextPoint);
}

Company::~Company() {
    // Clean up dynamically allocated market generator
    if (market != nullptr) {
        delete market;
        market = nullptr;
    }

    // Clean up current market data
    if (currentMarketData != nullptr) {
        delete currentMarketData;
        currentMarketData = nullptr;
    }

    // Optionally clear historical market data (not necessary if it's a vector of values, not pointers)
    previousValues.clear();
}

float Company::GetCurrentPrice() const {
    if (currentMarketData == nullptr) return 0.0f; // Failsafe: no current market data
    return currentMarketData->stockPrice;
}

float Company::CalculateIncreaseFromWeeksAgo(int weeksAgo) {
    if (previousValues.empty() || !currentMarketData)
        return 0.0f;  // Failsafe: no data available

    int currentMonth = GameState::Instance().GetMonth();
    int currentWeek = GameState::Instance().GetWeek();

    auto [startMonth, startWeek] = GetPastDate(currentMonth, currentWeek, weeksAgo);
    float priceStart = -1.0f;

    // Find the price at the start date
    for (const MarketData* data : previousValues) {
        if (data->monthAcquired == startMonth && data->weekAcquired == startWeek) {
            priceStart = data->stockPrice;
            break;
        }
    }

    float priceEnd = GetCurrentPrice(); // Use safer accessor

    if (priceStart < 0.0f || priceEnd <= 0.0f)
        return 0.0f;

    return (priceEnd - priceStart) / priceStart * 100.0f;
}

float Company::CalculateAverageIncreaseOverWeeks(int totalWeeks) {
    if (previousValues.empty() || totalWeeks <= 0 || !currentMarketData)
        return 0.0f;

    int currentMonth = GameState::Instance().GetMonth();
    int currentWeek = GameState::Instance().GetWeek();

    float totalIncrease = 0.0f;
    int validPairs = 0;

    for (int i = 1; i <= totalWeeks; ++i) {
        auto [startMonth, startWeek] = GetPastDate(currentMonth, currentWeek, i);
        auto [endMonth, endWeek]     = GetPastDate(currentMonth, currentWeek, i - 1);

        float priceStart = -1.0f, priceEnd = -1.0f;

        for (const MarketData* data : previousValues) {
            if (data->monthAcquired == startMonth && data->weekAcquired == startWeek)
                priceStart = data->stockPrice;
            else if (data->monthAcquired == endMonth && data->weekAcquired == endWeek)
                priceEnd = data->stockPrice;
        }

        if (priceStart > 0.0f && priceEnd > 0.0f) {
            float percentChange = (priceEnd - priceStart) / priceStart * 100.0f;
            totalIncrease += percentChange;
            validPairs++;
        }
    }

    if (validPairs == 0)
        return 0.0f;

    return totalIncrease / validPairs;
}


std::string Company::GetName() {
    return companyName;
}

std::string Company::GetDescription() {
    return companyDescription;
}

std::string Company::GetStoreDescription() {
    return companyStoreDescription;
}
