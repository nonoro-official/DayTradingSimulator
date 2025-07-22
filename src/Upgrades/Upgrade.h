//
// Created by Noah Peñaranda on 7/21/2025.
//

#pragma once
#include <string>
#include <functional>
#include "GameState.h"

class Upgrade {
private:
    std::string name;
    int cost = 0;
    bool purchased = false;
    std::function<void(GameState&)> effect;
public:
    void setName(std::string& name);
    void setCost(int c);
    void setEffect(std::function<void(GameState)> e);

    bool tryPurchase(GameState& state);
    void display();
};
