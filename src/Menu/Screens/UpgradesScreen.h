//
// Created by Noah Peñaranda on 7/30/2025.
//

#pragma once
#include "Upgrades/UpgradeHandler.h"
#include "Classes/PlayerData.h"
#include "Objects/PopUpWindow.h"

class UpgradesScreen
{
private:
     UpgradeHandler* handler;
     PlayerData* player;
     PopUpWindow* popup;

     char searchText[32] = "Search...";
     bool isSearchFocused = false;

public:
     UpgradesScreen(UpgradeHandler* handlerRef, PlayerData* playerRef, PopUpWindow* popupRef);
     void Draw();
};
