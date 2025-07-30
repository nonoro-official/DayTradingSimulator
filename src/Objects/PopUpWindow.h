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

    // textbox
    char inputBuffer[32] = "0.0";
    bool isInputFocused = false;
public:
    void Show(const std::string& p, float durationSeconds = 1.0f);
    void Hide();
    bool IsVisible() const;
    void Draw();
    void DrawBuySellPopup(bool isBuyMode, bool& isVisible, Company* company, PlayerData& player);

    const char* GetInputBuffer() const;
};


