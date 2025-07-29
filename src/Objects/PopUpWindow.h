//
// Created by Noah Peñaranda on 7/29/2025.
//

#pragma once
#include <string>

class PopUpWindow
{
private:
    std::string message;
    bool isVisible = false;
    float remainingTime = 0.0f;
public:
    void Show(const std::string& msg, float durationSeconds = 4.0f);
    void Hide();
    void Draw();
};


