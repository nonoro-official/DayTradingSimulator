//
// Created by Noah Peñaranda on 7/21/2025.
//

#include "Upgrade.h"
#include <iostream>

void Upgrade::setName(const std::string& name)
{
    this->name = name;
};

void Upgrade::setCost(int cost)
{
    this->cost = cost;
};

void Upgrade::setEffect(std::function<void(GameState&)> effect)
{
    this->effect = std::move(effect);
};

const std::string& Upgrade::getName()
{
    return name;
}

int Upgrade::getCost()
{
    return cost;
}

bool Upgrade::isPurchased()
{
    return purchased;
}

bool Upgrade::tryPurchase(GameState& state)
{
    if (!purchased && state.cash >= cost)
    {
        state.cash -= cost;
        effect(state);
        purchased = true;
        return true;
    }
    return false;
}

void Upgrade::display()
{
    std::cout << "UPGRADE: " << name << " ($" << cost << ") " << (purchased ? "YES" : "") << std::endl;
}

