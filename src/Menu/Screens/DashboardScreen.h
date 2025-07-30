//
// Created by Noah Peñaranda on 7/30/2025.
//

#pragma once
#include "Classes/Company.h"
#include "Classes/PlayerData.h"
#include "Objects/PopUpWindow.h"
#include "Classes/Transactions/TransactionManager.h"
#include <vector>
#include <string>

class DashboardScreen {
    private:
    std::vector<Company*>* companies;
    int* selectedCompanyIndex;

    // Local dropdown state
    bool dropdownOpen = false;
    float dropdownOpenTime = 0.0f;
    std::string cachedDropdown;

    std::string prediction;
    PopUpWindow* popup;

    // Trade popup
    bool showBuyPopup = false;
    bool showSellPopup = false;
    char inputBuffer[16] = "0.0";  // Initial placeholder
    bool isInputFocused = false;

    std::string BuildCompanyDropdownString();
    void UpdatePrediction();

    public:
    DashboardScreen(std::vector<Company*>* companiesRef, int* selectedIndex, PopUpWindow* popupRef);
    void Update();
    bool CanTrade(Company* company);
    void Draw();
};

