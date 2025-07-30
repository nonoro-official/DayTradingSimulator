//
// Created by Noah Peñaranda on 7/30/2025.
//

#pragma once
#include <vector>
#include "Classes/Company.h"
#include "Classes/PlayerData.h"
#include "Objects/PopUpWindow.h"

class CompaniesScreen {
private:
    std::vector<Company*>* companies;
    char searchText[32] = "Search...";
    bool isSearchFocused = false;

    bool showBuyPopup = false;
    Company* popupCompany = nullptr;
    char inputBuffer[16] = "0.0";  // For entering investment amount

    ~CompaniesScreen();

public:
    CompaniesScreen(std::vector<Company*>* companiesRef);
    void Draw();

    Font descriptionFont;
};


