//
// Created by Noah Peñaranda on 7/30/2025.
//


#pragma once
#include "Classes/Transactions/TransactionRecord.h"
#include "Classes/Transactions/TransactionManager.h"

class HistoryScreen {
private:
    Vector2 scroll = {0, 0};
    char searchText[32] = "Search...";
    bool isSearchFocused = false;
    int selectedType = 0;

public:
    void Draw();
};


