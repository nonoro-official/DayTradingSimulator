#pragma once

struct GameState {
    // 💰 Economy
    int cash = 500;                 // Starting cash

    // 🕒 Trade Mechanics
    float executionDelay = 8.0f;    // Initial trade delay in seconds

    // 🔮 Upgrade Effects
    bool showPrediction = false;   // Enables trend hints
    int graphZoom = 1;             // Graph zoom level

    // 📊 Optional Future
    int numTrades = 0;
    int sharesHeld = 0;
    float avgBuyPrice = 0.0f;

    // You can also add stock references, timers, etc. later
};
