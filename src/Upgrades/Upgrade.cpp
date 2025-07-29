//
// Created by Noah Peñaranda on 7/21/2025.
//

#include "Upgrade.h"

void Upgrade::setName(const std::string& name)
{
    this->name = name;
};

void Upgrade::setDescription(const std::string& description)
{
    this->description = description;
}

void Upgrade::setCost(int cost)
{
    this->cost = cost;
};

void Upgrade::setEffect(std::function<void(PlayerData&)> effect)
{
    this->effect = std::move(effect);
};

const std::string& Upgrade::getName()
{
    return name;
}

const std::string& Upgrade::getDescription()
{
    return description;
}

int Upgrade::getCost()
{
    return cost;
}

bool Upgrade::isPurchased()
{
    return purchased;
}

bool Upgrade::tryPurchase(PlayerData& player)
{
    if (!purchased && player.cash >= cost)
    {
        player.cash -= cost;
        effect(player);
        purchased = true;
        return true;
    }
    return false;
}
