//
// Created by Noah Peñaranda on 7/30/2025.
//

#pragma once
#include "Classes/PlayerData.h"
#include "Classes/Stock.h"
#include "Classes/Company.h"
#include "Objects/PopUpWindow.h"
#include <vector>
#include <string>

class PortfolioScreen {
private:
    char searchText[32] = "Search...";
    bool isSearchFocused = false;
public:
    explicit PortfolioScreen(PopUpWindow* popupRef);
    void Draw();

    PopUpWindow* popup;
    Company* popupCompany = nullptr;
    bool showBuyPopup = false;
    bool showPopup = false;
    char inputBuffer[32] = "0.0";
};
