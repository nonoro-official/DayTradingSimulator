//
// Created by sonik on 7/24/2025.
//

#include "Company.h"

#include <iostream>

Company::Company(std::string companyName, std::string companyDescription, float baseStockPrice, float amplitude, float frequency, GraphDisplay* display) {
    this->companyName = companyName;
    this->companyDescription = companyDescription;
    this->baseStockPrice = baseStockPrice;

    // initialize market
    market = new GenerateRandomMarket(amplitude, frequency);
    market->InitializeMarket();

    // pre gen points

    // Add Listener
    this->display = display;
    GameState::Instance().AddTickListener([this]() {
        GenerateNext(this->display);

        /*std::cout << "CurrentPrice: " << this->GetCurrentPrice() << " Month: " << currentMarketData->monthAcquired << " Week: " << currentMarketData->weekAcquired << std::endl;
        std::cout << "Increase: " << CalculateIncrease(0, 0, GameState::Instance().GetMonth(), GameState::Instance().GetWeek()) << "%" << std::endl;
    */
    });
}

void Company::GenerateNext(GraphDisplay* display) {
    // Generate point
    GraphPoint* nextPoint = market->GenerateNextPoint();

    // Create data
    MarketData* newData = new MarketData();
    newData->company = this;
    newData->monthAcquired = GameState::Instance().GetMonth();
    newData->weekAcquired = GameState::Instance().GetWeek();

    float multiplier = nextPoint->position.y;
    newData->stockPrice = this->baseStockPrice * multiplier;

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

float Company::GetCurrentPrice() const { return baseStockPrice * currentMarketData->stockPrice; }

float Company::CalculateIncrease(int startMonth, int startWeek, int endMonth, int endWeek) {
    float priceStart = -1.0f;
    float priceEnd = -1.0f;

    for (const MarketData* data : previousValues) {
        if (data->monthAcquired == startMonth && data->weekAcquired == startWeek) {
            priceStart = data->stockPrice;
        }
        if (data->monthAcquired == endMonth && data->weekAcquired == endWeek) {
            priceEnd = data->stockPrice;
        }
    }

    // Validate
    if (priceStart < 0.0f || priceEnd < 0.0f || priceStart == 0.0f) {
        return 0.0f; // Can't compute
    }

    float increase = (priceEnd - priceStart) / priceStart * 100.0f;
    return increase;
}