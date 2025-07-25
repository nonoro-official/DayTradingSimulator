#include "GenerateRandomMarket.h"

#include <iostream>
#include <bits/ostream.tcc>
#include <cstdlib>  // for rand()
#include <ctime>    // for time()


GenerateRandomMarket::GenerateRandomMarket(float amplitude, float frequency)
    : amplitude(amplitude), frequency(frequency) {
    srand(static_cast<unsigned int>(GetFrameTime())); // seed rand()
}

void GenerateRandomMarket::InitializeMarket() {
    perlinNoise = siv::PerlinNoise(seed);  // Initialize with seed
    time = 0.0f;
    generatedPoints.clear();

    if (OnFinishInitialize) OnFinishInitialize();
}

const std::vector<GraphPoint>& GenerateRandomMarket::GetMarketValues() const {
    return generatedPoints;
}

GraphPoint* GenerateRandomMarket::GenerateNextPoint() {
    // Occasionally change the trend
    if (trendDuration <= 0.0f && ((rand() % 1000) / 1000.0f) < trendChangeChance) {
        // Random new trend direction, either fully up or fully down
        bool trendUp = rand() % 2 == 0;
        float strength = ((rand() % 51) + 10) / 1000.0f; // Range: [0.01, 0.06]
        trendStrength = trendUp ? strength : -strength;
        trendDuration = (rand() % 20) + 10;
    }

    if (trendDuration > 0.0f)
        trendDuration -= 1.0f;

    // Generate base Perlin noise
    float noise = perlinNoise.noise1D_01(time * frequency);  // 0–1 range
    float baseValue = noise * amplitude;

    // Add trend and volatility
    float trendOffset = time * trendStrength;

    // Volatility: random fluctuation between -1 and +1 scaled by volatility
    float volatilityOffset = ((rand() % 2001) - 1000) / 1000.0f * volatility;

    float finalValue = baseValue + trendOffset + volatilityOffset;

    GraphPoint* newPoint = new GraphPoint();
    newPoint->position = { time, finalValue };
    newPoint->prevPoint = nullptr;

    if (!generatedPoints.empty()) {
        newPoint->prevPoint = &generatedPoints.back();
    }

    generatedPoints.push_back(*newPoint);
    time += 0.5f;

    return newPoint;
}
