//
// Created by Noah Peñaranda on 7/21/2025.
//

#include "Upgrade.h"
#include <iostream>

void Upgrade::setName(std::string& n)
{
    this->name = n;
};

void Upgrade::setCost(int c)
{
    this->cost = c;
};

void Upgrade::setEffect(std::function<void(GameState)> e)
{
    this->effect = std::move(e);
};

bool Upgrade::tryPurchase(GameState& state)
{
    if (!purchased && state.cash >= this->cost)
    {
        state.cash -= this->cost;
        effect(state);
        purchased = true;
        return true;
    }
    return false;
}

void Upgrade::display()
{
    std::cout << "UPGRADE: " << this->name << " ($" << this->cost << ")" << std::endl;
}

