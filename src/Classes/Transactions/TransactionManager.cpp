//
// Created by sonik on 7/30/2025.
//

#include "TransactionManager.h"
#include "Classes/PlayerData.h"

void TransactionManager::Update() {
    // Process Buy Orders
    for (int i = static_cast<int>(buyOrders.size()) - 1; i >= 0; --i) {
        BuyOrder& order = buyOrders[i];
        order.timeLeft--;

        if (order.timeLeft <= 0 && order.stock) {
            order.stock->BuyStock(order.cashDeposited);
            buyOrders.erase(buyOrders.begin() + i);
        }
    }

    // Process Sell Orders
    for (int i = static_cast<int>(sellOrders.size()) - 1; i >= 0; --i) {
        SellOrder& order = sellOrders[i];
        order.timeLeft--;

        if (order.timeLeft <= 0 && order.stock) {
            order.stock->SellStock(order.unitsWithdrawn);
            sellOrders.erase(sellOrders.begin() + i);
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