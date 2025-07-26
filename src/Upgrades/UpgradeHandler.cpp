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
        .setCost(200)
        .setEffect([](PlayerData& player)
        {
            player.showPrediction = true;
            std::cout << "Prediction Hints activated!" << std::endl;
        })
        .build());

    upgrades.push_back(
        builder.setName("Graph Zoom")
        .setCost(250)
        .setEffect([](PlayerData& player)
        {
            player.graphZoom++;
            std::cout << "Graph Zoom level: " << player.graphZoom << std::endl;
        })
        .build());

}

void UpgradeHandler::showAvailable()
{
    std::cout << "Available Upgrades:" << std::endl;
    for (size_t i = 0; i < upgrades.size(); i++)
    {
        auto& u = upgrades[i];
        std::cout << "[" << i << "] "
                  << std::setw(25) << std::left << u.getName()
                  << "$" << std::setw(4) << u.getCost()
                  << (u.isPurchased() ? " YES" : "") << std::endl;
    }
}

void UpgradeHandler::handlePurchase(int index, PlayerData& player)
{
    if (index < 0 || index >= upgrades.size())
    {
        std::cout << "Invalid index!" << std::endl;
        return;
    }

    if (!upgrades[index].tryPurchase(player))
    {
        std::cout << "Could not purchase." << std::endl;
    }
}
