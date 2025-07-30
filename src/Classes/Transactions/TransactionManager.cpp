//
// Created by sonik on 7/30/2025.
//

#include "TransactionManager.h"
#include "Classes/PlayerData.h"

void TransactionManager::Update() {
    // Process buy orders
    for (int i = 0; i < buyOrders.size(); ++i) {
        BuyOrder& order = buyOrders[i];
        order.timeLeft--;

        if (order.timeLeft <= 0) {
            float price = order.stock->company->GetCurrentPrice();
            float shares = std::round((order.cashDeposited / price) * 100.0f) / 100.0f;
            order.stock->BuyStock(order.cashDeposited);

            transactionHistory.emplace_back(
                TransactionRecord::Buy, order.stock, shares, price, shares * price
            );

            buyOrders.erase(buyOrders.begin() + i);
            --i;
        }
    }

    // Process sell orders
    for (int i = 0; i < sellOrders.size(); ++i) {
        SellOrder& order = sellOrders[i];
        order.timeLeft--;

        if (order.timeLeft <= 0) {
            float price = order.stock->company->GetCurrentPrice();
            float value = order.unitsWithdrawn * price;
            order.stock->SellStock(order.unitsWithdrawn);

            transactionHistory.emplace_back(
                TransactionRecord::Sell, order.stock, order.unitsWithdrawn, price, value
            );

            sellOrders.erase(sellOrders.begin() + i);
            --i;
        }
    }
}



void TransactionManager::CreateBuyOrder(Stock* stock, int delay, float cashDeposited) {
    if (!stock || cashDeposited <= 0) return;

    BuyOrder order;
    order.stock = stock;
    order.timeLeft = delay;
    order.cashDeposited = cashDeposited;

    PlayerData::Instance().cash -= cashDeposited;
    buyOrders.push_back(order);

    // === Record in Transaction History ===
    float price = stock->company->GetCurrentPrice();
    float estimatedShares = std::round((cashDeposited / price) * 100.0f) / 100.0f;

    transactionHistory.emplace_back(
        TransactionRecord::PlacedBuy, stock, estimatedShares, price, cashDeposited
    );
}


void TransactionManager::CreateSellOrder(Stock* stock, int delay, float unitsToSell) {
    if (!stock || unitsToSell <= 0 || stock->shares < unitsToSell) return;

    SellOrder order;
    order.stock = stock;
    order.timeLeft = delay;
    order.unitsWithdrawn = unitsToSell;

    order.stock->shares -= unitsToSell;
    sellOrders.push_back(order);

    // === Record in Transaction History ===
    float price = stock->company->GetCurrentPrice();
    float estimatedReturn = unitsToSell * price;

    transactionHistory.emplace_back(
        TransactionRecord::PlacedSell, stock, unitsToSell, price, estimatedReturn
    );
}

bool TransactionManager::HasPendingBuy(Stock* stock)
{
    for (auto& order : buyOrders)
    {
        if (order.stock == stock) return true;
    }
    return false;
}

bool TransactionManager::HasPendingSell(Stock* stock)
{
    for (auto& order : sellOrders)
    {
        if (order.stock == stock) return true;
    }
    return false;
}

bool TransactionManager::HasPendingOrder(Stock* stock) {
    return HasPendingBuy(stock) || HasPendingSell(stock);
}
