//
// Created by sonik on 7/24/2025.
//

#include "PlayerData.h"
#include "Stock.h"

PlayerData::PlayerData() {
    cash = 150;
    weekExecutionDelay = 4;
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
