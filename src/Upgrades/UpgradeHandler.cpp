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
        .setCost(200)
        .setEffect([](PlayerData& player)
        {
            PlayerData::Instance().UpgradeWeekExecution();
            std::cout << "Execution Speed improved: " << player.weekExecutionDelay << "weeks" << std::endl;
        })
        .build());

    upgrades.push_back(
        builder.setName("Prediction Hint")
        .setDescription("Enables market prediction leveraging cutting edge technology. ")
        .setCost(250)
        .setEffect([](PlayerData& player)
        {
            PlayerData::Instance().UpgradePrediction();
            std::cout << "Prediction Hints activated!" << std::endl;
        })
        .build());

    upgrades.push_back(
        builder.setName("Sell Bonus")
        .setDescription("Gain +0.1% more from stock sales. ")
        .setCost(300)
        .setEffect([](PlayerData& player)
        {
            PlayerData::Instance().sellBonusMultiplier += 0.1f;
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

void UpgradeHandler::progressWeek(PlayerData& player, PopUpWindow& popup)
{
    for (Upgrade& upgrade : upgrades)
    {
        if (upgrade.isPending())
        {
            upgrade.decrementPending();

            if (!upgrade.isPending())
            {
                int oldTier = upgrade.getTier();
                upgrade.applyEffect(player);
                int newTier = upgrade.getTier();

                // Show popup if we just reached a new tier (even max)
                if (newTier > oldTier && newTier <= upgrade.getMaxTier())
                {
                    std::ostringstream oss;
                    oss << "Upgrade '" << upgrade.getName() << "' is now active!";
                    popup.Show(oss.str(), 3.0f);
                }
            }
        }
    }
}

void UpgradeHandler::handlePurchase(int index, PlayerData& player, PopUpWindow& message)
{
    if (index < 0 || index >= (int)upgrades.size())
    {
        message.Show("Invalid upgrade index.", 2.0f);
        return;
    }

    Upgrade& upgrade = upgrades[index];
    int delayWeeks = player.weekExecutionDelay;

    if (!upgrade.tryPurchase(player, delayWeeks))
    {
        if (upgrade.isPending())
            message.Show("Upgrade already pending, please wait.", 2.0f);
        else
            message.Show("Could not purchase. \nInsufficient funds or max tier reached.", 2.0f);
    }
    else
    {
        message.Show("Purchase successful!", 2.0f);

        // Schedule second popup explaining the delay
        scheduleUpgradeEffectPopup = true;
        scheduledUpgradeIndex = index;
    }
}

void UpgradeHandler::updatePopups(PlayerData& player, PopUpWindow& message)
{
    if (!message.IsVisible() && scheduleUpgradeEffectPopup)
    {
        scheduleUpgradeEffectPopup = false;

        if (scheduledUpgradeIndex >= 0 && scheduledUpgradeIndex < (int)upgrades.size())
        {
            Upgrade& upgrade = upgrades[scheduledUpgradeIndex];
            int delayWeeks = player.weekExecutionDelay;

            std::ostringstream oss;
            oss << "Upgrade '" << upgrade.getName() << "' will take " << delayWeeks << " week"
                << (delayWeeks > 1 ? "s" : "") << " to take effect.";

            message.Show(oss.str(), 3.0f);
        }
    }
}
