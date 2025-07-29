//
// Created by Noah Peñaranda on 7/29/2025.
//

#pragma once
#include <string>

class MessageDisplay
{
private:
    std::string message;
    bool isVisible = false;
public:
    void Show(const std::string& msg);
    void Hide();
    void Draw();
};


