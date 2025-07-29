//
// Created by Noah Peñaranda on 7/30/2025.
//

#pragma once
#include "Classes/Company.h"
#include "Classes/PlayerData.h"
#include "Objects/PopUpWindow.h"
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

    std::string BuildCompanyDropdownString();

    bool showBuyPopup = false;
    int shareAmount = 0.5f;

    public:
    DashboardScreen(std::vector<Company*>* companiesRef, int* selectedIndex);
    void Update();
    void Draw();
    void DrawBuyStockPopup();
};

