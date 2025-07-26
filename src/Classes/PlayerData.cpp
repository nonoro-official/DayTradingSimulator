//
// Created by sonik on 7/24/2025.
//

#include "PlayerData.h"
#include "Stock.h"

PlayerData::PlayerData() {
    cash = 0;
    weekExecutionDelay = 4;
}

void PlayerData::AddStock(Stock& stock) {
    // Check if player already owns stock from this company
    for (Stock& s : stocks)
    {
        if (s.company == stock.company)
        {
            s.shares += stock.shares;
            return;
        }
    }
    // else add new stock
    stocks.push_back(stock);
}
bool PlayerData::SellStock(Company* company, float sharesToSell) {
    for (auto it = stocks.begin(); it != stocks.end(); ++it) {
        if (it->company == company) {
            if (it->shares >= sharesToSell) {
                it->shares -= sharesToSell;
                if (it->shares == 0) {
                    stocks.erase(it);
                }
                return true;
            }
            return false; // not enough shares
        }
    }
    return false; // no stock found
}

float PlayerData::GetTotalShares(Company* company) {
    for (const Stock& s : stocks) {
        if (s.company == company) {
            return s.shares;
        }
    }
    return 0.0f;
}

std::vector<Stock>& PlayerData::GetStocks() {
    return stocks;
}

float PlayerData::GetTotalPortfolioValue()
{
    float total = 0.0f;
    for (Stock& s : stocks)
    {
        total += s.GetShareValue();
    }
    return total;
}

float PlayerData::GetTotalProfitLoss(Company* company, float avgBuyPrice) {
    float totalShares = GetTotalShares(company);
    float currentPrice = company->GetCurrentPrice();
    return (currentPrice - avgBuyPrice) * totalShares;
}