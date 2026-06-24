//
// Created by Noah Peñaranda on 7/21/2025.
//

#pragma once
#include <string>
#include <functional>
#include "Classes/PlayerData.h"

class Upgrade
{
private:
    std::string name;
    std::string description;
    int cost = 0;
    int tier = 0;
    int maxTier = 3;
    std::function<void(PlayerData&)> effect;
    int pendingWeeks = 0;

public:
    Upgrade() = default;

    void setName(const std::string& name);
    void setDescription(const std::string& description);
    void setCost(int cost);
    void setEffect(std::function<void(PlayerData&)> effect);

    const std::string& getName();
    const std::string& getDescription();
    int getCost();
    bool isPurchased();
    int getTier();
    int getMaxTier();

    bool isPending() const;
    void setPending(int weeks);
    void decrementPending();
    int getPendingWeeks() const;

    bool tryPurchase(PlayerData& player, int delayWeeks);
    void applyEffect(PlayerData& player);
};
