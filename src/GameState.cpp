// src/GameState.cpp

#include "GameState.h"

bool GameState::IsPaused() {
    return paused;
}

float GameState::GetTimeScale() {
    return timeScale;
}

void GameState::PauseGame() {
    paused = !paused;
    timeScale = 1.0f;
}

void GameState::SetPause(bool value) {
    paused = value;
}

void GameState::SetTimeScale(float scale) {
    SetPause(false);
    timeScale = scale;
}

float GameState::GetTotalProfitLoss(float currentPrice) {
    float currentValue = sharesHeld * currentPrice;
    float costBasis = sharesHeld * avgBuyPrice;
    return currentValue - costBasis;
}