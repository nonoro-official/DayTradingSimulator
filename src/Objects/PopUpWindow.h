//
// Created by Noah Peñaranda on 7/29/2025.
//

#pragma once
#include "Classes/PlayerData.h"
#include "Classes/Company.h"
#include "Classes/Stock.h"
#include <string>

class PopUpWindow
{
private:
    std::string popUp;
    bool isVisible = false;
    float remainingTime = 0.0f;
public:
    void Show(const std::string& p, float durationSeconds = 2.0f);
    void Hide();
    void Draw();
    void DrawBuySellPopup(bool isBuyMode, bool& isVisible, Company* company, PlayerData& player, char* inputText);
};


