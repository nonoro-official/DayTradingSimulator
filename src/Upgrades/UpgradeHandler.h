//
// Created by Noah Peñaranda on 7/22/2025.
//

#pragma once
#include <vector>
#include "Upgrade.h"
#include "../Classes/GameState.h"

class UpgradeHandler
{
private:
    std::vector<Upgrade> upgrades;

public:
    void init(GameState& game);
    void showAvailable();
    void handlePurchase(int index, PlayerData& player);
};
