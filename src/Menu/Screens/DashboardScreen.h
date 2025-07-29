//
// Created by Noah Peñaranda on 7/30/2025.
//

#pragma once
#include "Classes/Company.h"
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

    public:
    DashboardScreen(std::vector<Company*>* companiesRef, int* selectedIndex);
    void Update();
    void Draw();
};

