// src/GameState.cpp

#include "GameState.h"

#include "raylib.h"

bool GameState::IsPaused() {
    return paused || tempPaused;
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

void GameState::SetTempPause(bool value) {
    tempPaused = value;
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

void GameState::AddWeek() {
    week++;
    if (week == 4) {
        month++;
        week = 0;
    }
}


int GameState::GetMonth() { return this->month; }

int GameState::GetWeek() { return this->week; }

void GameState::Update() {
    if (paused || tempPaused) return;

    tickTimer += GetFrameTime();

    if (tickTimer >= tickInterval * (1.0f / timeScale)) {
        tickTimer = 0.0f;

        // Notify listeners
        for (auto& listener : listeners) {
            listener();
        }

        // Increment week
        AddWeek();
    }
}

void GameState::AddTickListener(std::function<void()> listener) {
    listeners.push_back(listener);
}