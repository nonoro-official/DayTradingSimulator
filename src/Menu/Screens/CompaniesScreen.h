//
// Created by Noah Peñaranda on 7/30/2025.
//

#pragma once
#include <vector>
#include "Classes/Company.h"

class CompaniesScreen {
private:
    std::vector<Company*>* companies;
    char searchText[32] = "Search...";
    bool isSearchFocused = false;
public:
    CompaniesScreen(std::vector<Company*>* companiesRef);
    void Draw();
};


