//
// Created by Noah Peñaranda on 7/22/2025.
//

#include "UpgradeHandler.h"
#include "UpgradeBuilder.h"
#include <iostream>
#include <iomanip>

void UpgradeHandler::init(GameState& game)
{
    UpgradeBuilder builder;

    upgrades.push_back(
        builder.setName("Faster Execution")
        .setDescription("Reduces transaction delay upon purchase. ")
        .setCost(150)
        .setEffect([](PlayerData& player)
        {
            player.UpgradeWeekExecution();
            std::cout << "Execution Speed improved: " << player.weekExecutionDelay << "weeks" << std::endl;
        })
        .build());

    upgrades.push_back(
        builder.setName("Prediction Hint")
        .setDescription("Enables market prediction leveraging cutting edge technology. ")
        .setCost(200)
        .setEffect([](PlayerData& player)
        {
            player.UpgradePrediction();
            std::cout << "Prediction Hints activated!" << std::endl;
        })
        .build());

    upgrades.push_back(
        builder.setName("Sell Bonus")
        .setDescription("Gain +0.1% more from stock sales. ")
        .setCost(250)
        .setEffect([](PlayerData& player)
        {
            player.sellBonusMultiplier += 0.001f;
            std::cout << "Sell Bonus active! New multiplier: "
                      << std::fixed << std::setprecision(3)
                      << player.sellBonusMultiplier << "x" << std::endl;
        })
        .build());

}

std::vector<Upgrade>& UpgradeHandler::getUpgrades()
{
    return upgrades;
}

void UpgradeHandler::handlePurchase(int index, PlayerData& player, PopUpWindow& message)
{
    if (index < 0 || index >= upgrades.size())
    {
        message.Show("Invalid index.");
        return;
    }

    Upgrade& upgrade = upgrades[index];

    if (!upgrade.tryPurchase(player))
    {
        message.Show("Could not purchase. \nInsufficient funds or already purchased.");
    }
    else
    {
        PlayerData::Instance().cash -= upgrade.getCost();
        message.Show("Purchase successful!");
    }
}

