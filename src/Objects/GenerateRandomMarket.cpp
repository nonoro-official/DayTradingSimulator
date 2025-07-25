#include "GenerateRandomMarket.h"

#include <chrono>
#include <iostream>
#include <bits/ostream.tcc>
#include <cstdlib>  // for rand()
#include <ctime>    // for time()

#include "raymath.h"


GenerateRandomMarket::GenerateRandomMarket(float amplitude, float frequency)
    : amplitude(amplitude), frequency(frequency) {
    unsigned int seed = static_cast<unsigned int>(
    std::chrono::high_resolution_clock::now().time_since_epoch().count());
    srand(seed); // seed rand()
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
    // If in hold mode, repeat last value
    if (holdStepsRemaining > 0) {
        holdStepsRemaining--;

        float holdNoise = perlinNoise.noise1D_01(time * frequency);
        float holdValue = holdNoise * amplitude * holdNoiseDampening;
        holdValue = Clamp(holdValue, 0.0f, 1.0f);

        GraphPoint* newPoint = new GraphPoint();
        newPoint->position = { time, holdValue };
        newPoint->prevPoint = &generatedPoints.back();

        generatedPoints.push_back(*newPoint);
        time += 0.5f;
        return newPoint;
    }

    // Chance to enter hold mode
    float randomHold = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
    if (randomHold < holdChance) {
        float holdMin = holdDurationRange.x;
        float holdMax = holdDurationRange.y;
        float durationRand = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
        holdStepsRemaining = static_cast<int>(holdMin + (holdMax - holdMin) * durationRand);

        // First frame of hold: still emit this current value
        if (!generatedPoints.empty()) {
            GraphPoint* newPoint = new GraphPoint();
            newPoint->position = { time, generatedPoints.back().position.y };
            newPoint->prevPoint = &generatedPoints.back();

            generatedPoints.push_back(*newPoint);
            time += 0.5f;
            return newPoint;
        }
    }

    // ---- Existing trend logic below ----

    // Trend decision logic...
    if (activeTrendDuration <= 0.0f && ((rand() % 1000) / 1000.0f) < trendChangeChance) {
        float randFloat = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);

        if (randFloat < trendMaintainChance) {
            float minDuration = trendDurationRange.x;
            float maxDuration = trendDurationRange.y;
            float durationRand = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
            activeTrendDuration = minDuration + (maxDuration - minDuration) * durationRand;
        } else {
            float minTrend = trendStrengthRange.x;
            float maxTrend = trendStrengthRange.y;
            float trendRand = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
            activeTrendStrength = minTrend + (maxTrend - minTrend) * trendRand;

            float minDuration = trendDurationRange.x;
            float maxDuration = trendDurationRange.y;
            float durationRand = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
            activeTrendDuration = minDuration + (maxDuration - minDuration) * durationRand;
        }
    }

    if (activeTrendDuration > 0.0f)
        activeTrendDuration -= 1.0f;

    float noise = perlinNoise.noise1D_01(time * frequency);
    float baseValue = noise * amplitude;

    float trendOffset = time * activeTrendStrength;
    float volatilityOffset = ((rand() % 2001) - 1000) / 1000.0f * volatility;

    float finalValue = baseValue + trendOffset + volatilityOffset;

    // Clamp
    finalValue = Clamp(finalValue, 0.0f, 1.0f);

    // Failsafe
    if (finalValue <= 0.0f || finalValue >= 1.0f) {
        activeTrendStrength *= -1.0f;
        activeTrendDuration = 5.0f;
    }

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
