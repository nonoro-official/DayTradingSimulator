//
// Created by Noah Peñaranda on 7/21/2025.
//

#pragma once
#include <string>
#include <functional>
#include "GameState.h"

class Upgrade
{
private:
    std::string name;
    int cost = 0;
    bool purchased = false;
    std::function<void(GameState&)> effect;

public:
    Upgrade() = default;

    void setName(const std::string& name);
    void setCost(int cost);
    void setEffect(std::function<void(GameState&)> effect);

    const std::string& getName();
    int getCost();
    bool isPurchased();

    bool tryPurchase(GameState& state);
    void display();
};
