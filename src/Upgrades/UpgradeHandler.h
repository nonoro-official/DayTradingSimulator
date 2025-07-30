//
// Created by Noah Peñaranda on 7/22/2025.
//

#pragma once
#include <vector>
#include "Upgrade.h"
#include "Classes/GameState.h"
#include "Objects/PopUpWindow.h"

class UpgradeHandler
{
private:
    std::vector<Upgrade> upgrades;
    bool scheduleUpgradeEffectPopup = false;
    int scheduledUpgradeIndex = -1;

public:
    void init(GameState& game);
    void handlePurchase(int index, PlayerData& player, PopUpWindow& message);
    void updatePopups(PlayerData& player, PopUpWindow& message);
    std::vector<Upgrade>& getUpgrades();
};
