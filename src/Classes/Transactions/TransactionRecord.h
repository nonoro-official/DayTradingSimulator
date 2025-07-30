// TransactionRecord.h
#ifndef TRANSACTIONRECORD_H
#define TRANSACTIONRECORD_H

#include "Stock.h"

struct TransactionRecord {
    enum Type {
        Buy,
        Sell,
        PlacedBuy,
        PlacedSell
    };

    Stock* stock;
    float shares;
    float pricePerShare;
    float totalAmount;
    int month;
    int week;

    TransactionRecord(Type t, Stock* s, float sh, float price, float total)
        : type(t), stock(s), shares(sh), pricePerShare(price), totalAmount(total) {
        month = GameState::Instance().GetMonth();
        week = GameState::Instance().GetWeek();
    }
};


#endif
