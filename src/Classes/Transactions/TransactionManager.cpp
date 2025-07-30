//
// Created by sonik on 7/30/2025.
//

#include "TransactionManager.h"
#include "Classes/PlayerData.h"

void TransactionManager::Update() {
    // Process buy orders
    for (int i = 0; i < buyOrders.size(); ++i) {
        BuyOrder& order = buyOrders[i];
        order.delay--;

        if (order.delay <= 0) {
            float price = order.stock->company->GetCurrentPrice();
            float shares = std::round((order.deposit / price) * 100.0f) / 100.0f;
            order.stock->BuyStock(order.deposit);

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
        order.delay--;

        if (order.delay <= 0) {
            float price = order.stock->company->GetCurrentPrice();
            float value = order.units * price;
            order.stock->SellStock(order.units);

            transactionHistory.emplace_back(
                TransactionRecord::Sell, order.stock, order.units, price, value
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
}

void TransactionManager::CreateSellOrder(Stock* stock, int delay, float unitsToSell) {
    if (!stock || unitsToSell <= 0 || stock->shares < unitsToSell) return;

    SellOrder order;
    order.stock = stock;
    order.timeLeft = delay;
    order.unitsWithdrawn = unitsToSell;

    order.stock->shares -= unitsToSell;

    sellOrders.push_back(order);
}