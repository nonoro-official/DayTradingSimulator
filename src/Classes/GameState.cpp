// src/GameState.cpp

#include "Classes/GameState.h"
#include "Classes/Company.h"
#include "Classes/Stock.h"
#include "raylib.h"
#include "PlayerData.h"

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

void GameState::AddWeek() {
    week++;
    if (week == 5) {
        month++;
        week = 1;
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

void GameState::InitializeCompaniesAndStocks() {
    Company* lemon = new Company("Lemon Inc", "Develops sustainable tech for modern agriculture.", 0.6f, 0.9f, new GraphDisplay({540, 300}, {840, 360}));
    lemon->market->SetNormalValues(GenerateRandomMarket::PerlinNoise, 0.3f);
    lemon->market->SetTrendValues(GenerateRandomMarket::PerlinNoise, 0.25f, {8, 20}, {-0.01f, 0.02f});
    lemon->market->SetHoldValues(GenerateRandomMarket::WhiteNoise, 0.05f, {6, 12});
    lemon->market->SetVolatileValues(GenerateRandomMarket::WhiteNoise, 0.8f, {3, 6});
    lemon->SetStoreValues(
        "A slow-growing tech firm focused on sustainable agriculture and automation.\n"
        "Stable and low-risk, it’s ideal for long-term investors looking for consistent gains.",
        52.00f
    );
    companies.push_back(lemon);

    Company* banana = new Company("Banana Corp", "Builds AI systems for automated fruit farming.", 1.25f, 1.4f, new GraphDisplay({540, 300}, {840, 360}));
    banana->market->SetNormalValues(GenerateRandomMarket::WhiteNoise, 0.6f);
    banana->market->SetTrendValues(GenerateRandomMarket::WhiteNoise, 0.5f, {3, 8}, {-0.04f, 0.06f});
    banana->market->SetHoldValues(GenerateRandomMarket::WhiteNoise, 0.15f, {2, 5});
    banana->market->SetVolatileValues(GenerateRandomMarket::WhiteNoise, 1.6f, {4, 10});
    banana->SetStoreValues(
        "A volatile AI startup blending biotech with fruit farming.\n"
        "High-risk, high-reward — perfect for traders chasing sharp spikes and quick flips.",
        38.00f
    );
    companies.push_back(banana);

    Company* mango = new Company("Mango Ltd", "Manages cargo and logistics across tropical regions.", 0.9f, 1.1f, new GraphDisplay({540, 300}, {840, 360}));
    mango->market->SetNormalValues(GenerateRandomMarket::PerlinNoise, 0.4f);
    mango->market->SetTrendValues(GenerateRandomMarket::WhiteNoise, 0.3f, {6, 14}, {-0.015f, 0.035f});
    mango->market->SetHoldValues(GenerateRandomMarket::WhiteNoise, 0.1f, {4, 10});
    mango->market->SetVolatileValues(GenerateRandomMarket::PerlinNoise, 1.2f, {5, 12});
    mango->SetStoreValues(
        "A dependable logistics company serving tropical trade routes.\n"
        "Moderately stable with occasional bursts, it's best bought before seasonal demand ramps up.",
        46.00f
    );
    companies.push_back(mango);

    // Add starter stocks to player
    for (Company* company : companies) {
        Stock* stock = new Stock(*company, 0.5f);
        PlayerData::Instance().AddStock(stock);
    }
}

std::vector<Company*>& GameState::GetCompanies() {
    return companies;
}

int& GameState::GetSelectedCompanyIndex() {
    return selectedCompanyIndex;
}

Company* GameState::GetCompanyByIndex(int index) {
    if (index >= 0 && index < companies.size()) {
        return companies[index];
    }
    return nullptr;
}

Stock* GameState::GetStockByCompany(Company* company) {
    for (Stock* stock : PlayerData::Instance().GetStocks()) {
        if (stock->company == company)
            return stock;
    }
    return nullptr;
}

Stock* GameState::GetStockByCompanyIndex(int index) {
    Company* company = GetCompanyByIndex(index);
    if (company)
        return GetStockByCompany(company);
    return nullptr;
}
