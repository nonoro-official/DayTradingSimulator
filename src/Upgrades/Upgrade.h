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
    double cost;
    bool purchased;
    std::function<void(GameState&)> effect;
public:
    void setName(std::string name);
    void setCost(double cost);
    void setEffect();
};
