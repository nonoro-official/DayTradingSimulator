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
        .setDescription("Reduces transaction delay upon purchase. Current transaction delay: " + std::to_string(PlayerData::Instance().weekExecutionDelay))
        .setCost(150)
        .setEffect([](PlayerData& player)
        {
            player.weekExecutionDelay -= 1;
            if (player.weekExecutionDelay < 1) player.weekExecutionDelay = 1;
            std::cout << "Execution Speed improved: " << player.weekExecutionDelay << "s" << std::endl;
        })
        .build());

    upgrades.push_back(
        builder.setName("Prediction Hint")
        .setDescription("Enables market prediction leveraging cutting edge technology. Current level: " + std::to_string(PlayerData::Instance().showPredictionTier))
        .setCost(200)
        .setEffect([](PlayerData& player)
        {
            player.UpgradePrediction();
            std::cout << "Prediction Hints activated!" << std::endl;
        })
        .build());

    upgrades.push_back(
        builder.setName("Graph Zoom")
        .setDescription("description 3")
        .setCost(250)
        .setEffect([](PlayerData& player)
        {
            player.graphZoom++;
            std::cout << "Graph Zoom level: " << player.graphZoom << std::endl;
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

    if (!upgrades[index].tryPurchase(player))
    {
        message.Show("Could not purchase. \nInsufficient funds or already purchased.");
    }
    else
    {
        // Deduct the cost after a successful check
        PlayerData::Instance().cash -= upgrade.getCost();
        message.Show("Purchase successful!");
    }

}
