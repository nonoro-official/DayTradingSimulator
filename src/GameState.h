#pragma once

struct GameState {
    int cash = 500;                 // Starting cash

    float executionDelay = 8.0f;    // Initial trade delay in seconds

    // Upgrade effects
    bool showPrediction = false;   // Enables trend hints
    int graphZoom = 1;             // Graph zoom level

    int numTrades = 0;
    int sharesHeld = 0;
    float avgBuyPrice = 0.0f;

};
