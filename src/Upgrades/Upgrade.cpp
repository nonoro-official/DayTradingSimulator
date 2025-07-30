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

bool Upgrade::isPending() const
{
    return pendingWeeks > 0;
}

void Upgrade::setPending(int weeks)
{
    pendingWeeks = weeks;
}

void Upgrade::decrementPending()
{
    if (pendingWeeks > 0)
        pendingWeeks--;
}

int Upgrade::getPendingWeeks() const
{
    return pendingWeeks;
}

bool Upgrade::tryPurchase(PlayerData& player, int delayWeeks)
{
    if (player.cash >= cost && tier < maxTier && !isPending())
    {
        player.cash -= cost;
        pendingWeeks = delayWeeks;  // Start countdown
        // effect will be applied later, after countdown
        return true;
    }
    return false;
}

void Upgrade::applyEffect(PlayerData& player) {
    if (effect) effect(player);
    tier++;
    pendingWeeks = 0;
}




