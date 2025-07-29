#include "GenerateRandomMarket.h"

#include <chrono>
#include <iostream>
#include <bits/ostream.tcc>
#include <cstdlib>  // for rand()
#include <ctime>    // for time()

#include "raymath.h"

float GetRandomFloat(float min, float max) {
    return min + (float)GetRandomValue(0, 10000) / 10000.0f * (max - min);
}

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

const std::vector<GraphPoint*>& GenerateRandomMarket::GetMarketValues() const {
    return generatedPoints;
}

void GenerateRandomMarket::SetMarketState(MarketState state) {
    currentState = state;

    switch (currentState) {
        case Normal: break;

        case TrendUp: {
            timeInState = GetRandomValue(trendTimeRange.x, trendTimeRange.y);
        } break;

        case TrendDown: {
            timeInState = GetRandomValue(trendTimeRange.x, trendTimeRange.y);
        } break;

        case Hold: {
            timeInState = GetRandomValue(holdTimeRange.x, holdTimeRange.y);
        } break;

        case Volatile: {
            timeInState = GetRandomValue(volatileTimeRange.x, volatileTimeRange.y);
        } break;

        case Cooldown: {
            currentState = Cooldown;

            timeInState = GetRandomValue(eventCooldownRange.x, eventCooldownRange.y);
        } break;
    }
}

GraphPoint* GenerateRandomMarket::GenerateNextPoint() {

    char* MarketStateNames[] = {
        "Normal", "TrendUp", "TrendDown", "Hold", "Volatile", "Cooldown"};

    if (timeInState <= 0) {
        if (currentState != Cooldown) {
            SetMarketState(Cooldown);
        }

        if (currentState == Cooldown) {
            SetMarketState(Normal);
        }
    }
    else timeInState--;

    std::cout << std::endl << MarketStateNames[static_cast<int>(currentState)] << std::endl << "timeleft: " << timeInState << std::endl;

    // Events
    if (currentState == Normal) {
        // Event Selection
        float chance = static_cast<float>(rand()) / RAND_MAX;
        if (chance >= eventChance) {
            // chooseRandomEvent
            MarketState randomState = static_cast<MarketState>(GetRandomValue(1, 4));

            SetMarketState(randomState);
        }
    }

    if (currentState != Cooldown) {
        // Event Loop
        switch (currentState) {
            case TrendUp: {
                currentValue += GetRandomFloat(randomTrendStrength.x, 0);
            } break;

            case TrendDown: {
                currentValue += GetRandomFloat(0, randomTrendStrength.y);
            } break;

            case Hold: {

            } break;

            case Volatile: {

            } break;
        }
    }


    // Noise
    NoiseType noiseType = PerlinNoise;
    float noiseValue = 1;
    float noiseMultiplier = 1;
    switch (currentState) {
        default: noiseType = defaultNoiseType; noiseMultiplier = defaultNoiseMultiplier; break;
        case TrendUp: noiseType = trendNoiseType; noiseMultiplier = trendNoiseMultiplier; break;
        case TrendDown: noiseType = trendNoiseType; noiseMultiplier = trendNoiseMultiplier; break;
        case Hold: noiseType = holdNoiseType; noiseMultiplier = holdNoiseMultiplier; break;
        case Volatile: noiseType = volatileNoiseType; noiseMultiplier = volatileNoiseMultiplier; break;
    }

    switch (noiseType) {
        case PerlinNoise: noiseValue = perlinNoise.noise1D(amplitude * time * frequency) * noiseMultiplier; break;
        case WhiteNoise: noiseValue = Remap(static_cast<float>(rand()) / RAND_MAX, 0, 1, -.5, .5) * noiseMultiplier;
    }


    // If we're hitting the edges, steer the market back
    if (currentState != Volatile && currentState != Hold && currentState != Cooldown) {
        if (currentValue >= 1.0f - edgePadding) {
            SetMarketState(TrendUp);
        } else if (currentValue <= 0.025f + edgePadding) {
            SetMarketState(TrendDown);
        }
    }

    float yValue = currentValue + (currentValue * noiseValue);

    // Clamp to minmax
    currentValue = Clamp(currentValue, 0.025, 1.0f);
    yValue = Clamp(yValue, 0.025f, 1.0f);

    // Apply value
    GraphPoint* newPoint = new GraphPoint(yValue);
    newPoint->prevPoint = nullptr;

    generatedPoints.push_back(newPoint);

    time += 1;
    return newPoint;
}