//
// Created by Noah Peñaranda on 7/23/2025.
//

#pragma once
#include "Upgrade.h"
#include <string>

class UpgradeBuilder
{
private:
    Upgrade upgrade;

public:
    UpgradeBuilder& setName(const std::string& name);
    UpgradeBuilder& setDescription(const std::string& description);
    UpgradeBuilder& setCost(int cost);
    UpgradeBuilder& setEffect(std::function<void(PlayerData&)> effect);

    Upgrade build();
};
