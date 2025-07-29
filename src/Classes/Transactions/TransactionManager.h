//
// Created by sonik on 7/30/2025.
//

#ifndef TRANSACTIONORDER_H
#define TRANSACTIONORDER_H
#include "BuyOrder.h"
#include "SellOrder.h"


class TransactionManager {
    public:
    // Access the singleton instance
    static TransactionManager& Instance() {
        static TransactionManager instance; // Guaranteed to be initialized only once
        return instance;
    }

    std::vector<BuyOrder> buyOrders;
    std::vector<SellOrder> sellOrders;

    TransactionManager(){};

    void Update();
    void CreateBuyOrder(Stock* stock, int delay, float cashDeposited);
    void CreateSellOrder(Stock* stock, int delay, float unitsToSell);
};



#endif //TRANSACTIONORDER_H
