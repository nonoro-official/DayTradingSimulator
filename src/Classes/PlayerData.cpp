//
// Created by sonik on 7/24/2025.
//

#include "PlayerData.h"

#include <iomanip>

#include "Stock.h"

PlayerData::PlayerData() {
    cash = 150;
    weekExecutionDelay = 6;

    showPredictionTier = 3;
}

float PlayerData::GetTotalPortfolioValue() {
    float total = 0.0f;
    for (Stock* s : stocks) {
        if (s->company && s->company->currentMarketData) {
            total += s->GetShareValue();
        }
    }
    return total;
}

float PlayerData::GetTotalProfitLoss(Company* company, float avgBuyPrice) {
    float totalShares = GameState::Instance().GetStockByCompany(company)->shares;
    float currentPrice = company->GetCurrentPrice();
    return (currentPrice - avgBuyPrice) * totalShares;
}

void PlayerData::AddStock(Stock* stock) {
    stocks.push_back(stock);
}

std::vector<Stock*>& PlayerData::GetStocks() {
    return stocks;
}

// Upgrades
void PlayerData::UpgradePrediction() {
    showPredictionTier++;
}

void PlayerData::UpgradeWeekExecution() {
    weekExecutionDelayTier++;

    switch (weekExecutionDelayTier) {
        case 1:
            weekExecutionDelay = 5;
            break;
        case 2:
            weekExecutionDelay = 4;
            break;
        case 3:
            weekExecutionDelay = 3;
            break;
    }
}

std::string PlayerData::GetMarketPrediction(Company *company) {
    std::string result = "No Prediction.";

    if (showPredictionTier == 0 || !company || !company->market)
        return result;

    int weeks = 0;
    float accuracy = 1.0f;

    switch (showPredictionTier) {
        case 1:
            weeks = 4;
            accuracy = 0.6f;
            break;
        case 2:
            weeks = 6;
            accuracy = 0.4f;
            break;
        case 3:
            weeks = 8;
            accuracy = 0.2f;
            break;
    }

    float predictedValue = company->market->PredictAverageOverWeeks(weeks, accuracy, company->currentMarketData->yValue);
    float currentValue = company->GetCurrentPrice(); // Assumes this exists

    if (currentValue <= 0.0f) return "Invalid current value.";

    std::ostringstream oss;
    oss << "Predicted: ";
    if (predictedValue >= 0)
        oss << "+" << std::fixed << std::setprecision(2) << predictedValue << "% rise";
    else
        oss << std::fixed << std::setprecision(2) << predictedValue << "% fall";

    oss << " over the next " << weeks << " weeks.";

    return oss.str();
}


