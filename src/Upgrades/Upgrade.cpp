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

int Upgrade::getTier()
{
    return tier;
}

int Upgrade::getMaxTier()
{
    return maxTier;
}

bool Upgrade::isPurchased()
{
    return tier >= maxTier;
}

bool Upgrade::tryPurchase(PlayerData& player)
{
    if (player.cash >= cost && tier < maxTier)
    {
        player.cash -= cost;
        effect(player);
        tier++;
        return true;
    }

    return false;
}




