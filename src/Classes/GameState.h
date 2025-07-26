#pragma once
#include <functional>

class GameState {
public:
    // Access the singleton instance
    static GameState& Instance() {
        static GameState instance; // Guaranteed to be initialized only once
        return instance;
    }

    int numTrades = 0;

    // Game state control
    bool IsPaused();
    float GetTimeScale();
    void PauseGame();
    void SetPause(bool value);
    void SetTempPause(bool value);
    void SetTimeScale(float scale);
    int GetMonth();
    int GetWeek();
    void AddWeek();

    void AddTickListener(std::function<void()> listener);

    void Update();

private:
    // Private constructor prevents external instancing
    GameState() = default;
    GameState(const GameState&) = delete;
    GameState& operator=(const GameState&) = delete;

    // Internal state
    bool paused = false;
    bool tempPaused = false;
    float timeScale = 1.0f;

    // Timer
    float tickInterval = 2.5f;
    float tickTimer = 0.0f;
    std::vector<std::function<void()>> listeners;

    int month = 0;
    int week = 0;
};
