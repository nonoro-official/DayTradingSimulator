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
    bool purchased = false;
    std::function<void(PlayerData&)> effect;

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

    bool tryPurchase(PlayerData& player);
};
