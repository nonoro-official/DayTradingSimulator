#pragma once

class GameState {
public:
    // Access the singleton instance
    static GameState& Instance() {
        static GameState instance; // Guaranteed to be initialized only once
        return instance;
    }

    // Public state variables
    int month = 0;
    int cash = 500;                 // Starting cash
    float executionDelay = 8.0f;    // Initial trade delay in seconds

    // Upgrade effects
    bool showPrediction = false;   // Enables trend hints
    int graphZoom = 1;             // Graph zoom level

    int numTrades = 0;
    int sharesHeld = 0;
    float avgBuyPrice = 0.0f;

    // Game state control
    bool IsPaused();
    float GetTimeScale();
    void PauseGame();
    void SetPause(bool value);
    void SetTimeScale(float scale);
    float GetTotalProfitLoss(float currentPrice);

private:
    // Private constructor prevents external instancing
    GameState() = default;
    GameState(const GameState&) = delete;
    GameState& operator=(const GameState&) = delete;

    // Internal state
    bool paused = false;
    float timeScale = 1.0f;
};
