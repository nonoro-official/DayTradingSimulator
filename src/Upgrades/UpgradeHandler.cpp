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
        .setDescription("description 1")
        .setCost(150)
        .setEffect([](PlayerData& player)
        {
            player.weekExecutionDelay -= 1.5f;
            if (player.weekExecutionDelay < 0.5f) player.weekExecutionDelay = 0.5f;
            std::cout << "weekExecution Speed improved: " << player.weekExecutionDelay << "s" << std::endl;
        })
        .build());

    upgrades.push_back(
        builder.setName("Prediction Hint")
        .setDescription("description 2")
        .setCost(200)
        .setEffect([](PlayerData& player)
        {
            player.showPrediction = true;
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

void UpgradeHandler::handlePurchase(int index, PlayerData& player, MessageDisplay& message)
{
    if (index < 0 || index >= upgrades.size())
    {
        message.Show("Invalid index.");
        return;
    }

    if (!upgrades[index].tryPurchase(player))
    {
        message.Show("Could not purchase. Insufficient funds or already purchased.");
    }
    else
    {
        message.Show("Purchase successful!");
    }

}
