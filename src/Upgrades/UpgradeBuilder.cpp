//
// Created by Noah Peñaranda on 7/23/2025.
//

#include "UpgradeBuilder.h"

UpgradeBuilder& UpgradeBuilder::setName(const std::string& name)
{
    upgrade.setName(name);
    return *this;
}

UpgradeBuilder& UpgradeBuilder::setCost(int cost)
{
    upgrade.setCost(cost);
    return *this;
}

UpgradeBuilder& UpgradeBuilder::setEffect(std::function<void(PlayerData&)> effect)
{
    upgrade.setEffect(std::move(effect));
    return *this;
}

Upgrade UpgradeBuilder::build()
{
    return upgrade;
}