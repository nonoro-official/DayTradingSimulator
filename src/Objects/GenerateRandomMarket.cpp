#include "GenerateRandomMarket.h"

#include <chrono>
#include <iostream>
#include <bits/ostream.tcc>
#include <cstdlib>  // for rand()
#include <ctime>    // for time()

#include "MonthDisplay.h"
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
    perlinNoise = siv::PerlinNoise(seed);  // Re-seed noise
    generatedPoints.clear();

    time = -25; // Start from negative to simulate historical data
    currentValue = GetRandomFloat(0.1f, 0.9f); // Start from a middle value
    currentState = Normal;

    // Temporarily simulate past history (but don't store)
    for (int i = 0; i < 25; ++i) {
        GenerateNextPoint(); // this increments time too
    }

    // Reinitialize values for forward simulation
    time = 0;
    currentValue = Clamp(currentValue, 0.025f, 0.95f);
    SetMarketState(Normal);

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
    if (timeInState <= 0) {
        if (currentState != Cooldown) {
            SetMarketState(Cooldown);
        }

        if (currentState == Cooldown) {
            SetMarketState(Normal);
        }
    }
    else timeInState--;

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
                currentValue += GetRandomFloat(0, randomTrendStrength.x);  // Correct: increase
            } break;

            case TrendDown: {
                currentValue += GetRandomFloat(-randomTrendStrength.y, 0); // Correct: decrease
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
        case WhiteNoise: noiseValue = Remap(static_cast<float>(rand()) / RAND_MAX, 0.025, 0.95, -.5, .5) * noiseMultiplier;
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
    currentValue = Clamp(currentValue, 0.025, 0.95);
    yValue = Clamp(yValue, 0.025, 0.95);

    // Apply value
    GraphPoint* newPoint = new GraphPoint(yValue);
    newPoint->prevPoint = nullptr;

    generatedPoints.push_back(newPoint);

    time += 1;
    return newPoint;
}

std::vector<GraphPoint*> GenerateRandomMarket::GeneratePrefillPoints(int count) {
    std::vector<GraphPoint*> prefillPoints;

    float backupTime = time;
    float backupValue = currentValue;
    MarketState backupState = currentState;
    int backupTimeInState = timeInState;

    // Temporarily shift time back
    time = -count;

    for (int i = 0; i < count; ++i) {
        GraphPoint* point = GenerateNextPoint();
        prefillPoints.push_back(point);
    }

    // Restore state
    time = backupTime;
    currentValue = backupValue;
    currentState = backupState;
    timeInState = backupTimeInState;

    // Clear these generated points from internal storage
    generatedPoints.erase(generatedPoints.end() - count, generatedPoints.end());

    return prefillPoints;
}


void GenerateRandomMarket::SetNormalValues(NoiseType noiseType, float noiseMultiplier) {
    defaultNoiseType = noiseType;
    defaultNoiseMultiplier = noiseMultiplier;
}

void GenerateRandomMarket::SetTrendValues(NoiseType noiseType, float noiseMultiplier, Vector2 timeRange, Vector2 trendStrengthRange) {
    trendNoiseType = noiseType;
    trendNoiseMultiplier = noiseMultiplier;
    trendTimeRange = timeRange;
    randomTrendStrength = trendStrengthRange;
}

void GenerateRandomMarket::SetHoldValues(NoiseType noiseType, float noiseMultiplier, Vector2 timeRange) {
    holdNoiseType = noiseType;
    holdNoiseMultiplier = noiseMultiplier;
    holdTimeRange = timeRange;
}

void GenerateRandomMarket::SetVolatileValues(NoiseType noiseType, float noiseMultiplier, Vector2 timeRange) {
    volatileNoiseType = noiseType;
    volatileNoiseMultiplier = noiseMultiplier;
    volatileTimeRange = timeRange;
}

void GenerateRandomMarket::SetCurrentValue(float value) {
    currentValue = value;
}


float GenerateRandomMarket::PredictAverageOverWeeks(int weeks, float variationAmount, float valueAtStart)
{
    if (weeks <= 0 || valueAtStart <= 0.0f) return 0.0f;

    int totalPoints = weeks * 7; // assuming 1 point per day
    float simulatedValue = currentValue;
    float sum = 0.0f;

    float fakeTime = time;
    int fakeTimeInState = timeInState;
    MarketState fakeState = currentState;

    for (int i = 0; i < totalPoints; ++i) {
        // Decrease time in state
        if (fakeTimeInState <= 0) {
            if (fakeState != Cooldown)
                fakeState = Cooldown;
            else
                fakeState = Normal;

            switch (fakeState) {
                case TrendUp:
                case TrendDown:
                case Hold:
                case Volatile:
                    fakeTimeInState = GetRandomValue(5, 20);
                    break;
                case Cooldown:
                    fakeTimeInState = GetRandomValue(3, 10);
                    break;
                default:
                    break;
            }
        } else {
            fakeTimeInState--;
        }

        // Chance of event from Normal
        if (fakeState == Normal) {
            float chance = static_cast<float>(rand()) / RAND_MAX;
            if (chance >= eventChance) {
                fakeState = static_cast<MarketState>(GetRandomValue(1, 4));
                fakeTimeInState = GetRandomValue(5, 20);
            }
        }

        // Apply trend
        if (fakeState == TrendUp)
            simulatedValue += GetRandomFloat(0, randomTrendStrength.x);
        else if (fakeState == TrendDown)
            simulatedValue += GetRandomFloat(-randomTrendStrength.y, 0);


        // Noise parameters
        NoiseType noiseType = defaultNoiseType;
        float noiseMultiplier = defaultNoiseMultiplier;

        switch (fakeState) {
            case TrendUp:
            case TrendDown:
                noiseType = trendNoiseType;
                noiseMultiplier = trendNoiseMultiplier;
                break;
            case Hold:
                noiseType = holdNoiseType;
                noiseMultiplier = holdNoiseMultiplier;
                break;
            case Volatile:
                noiseType = volatileNoiseType;
                noiseMultiplier = volatileNoiseMultiplier;
                break;
            default:
                break;
        }

        float noiseValue = 0;
        switch (noiseType) {
            case PerlinNoise:
                noiseValue = perlinNoise.noise1D(amplitude * fakeTime * frequency);
                break;
            case WhiteNoise:
                noiseValue = Remap(static_cast<float>(rand()) / RAND_MAX, 0.025f, 0.95f, -0.5f, 0.5f);
                break;
        }

        float yValue = simulatedValue + (simulatedValue * noiseValue * noiseMultiplier);

        // Clamp and accumulate
        yValue = Clamp(yValue, 0.025f, 0.95f);
        simulatedValue = Clamp(simulatedValue, 0.025f, 0.95f);
        sum += yValue;

        fakeTime += 1.0f;
    }

    float average = sum / totalPoints;

    // Apply variation (±percentage)
    float variation = GetRandomFloat(-variationAmount, variationAmount);
    float variedAverage = Clamp(average + (average * variation), 0.025f, 0.95f);

    // Return percent change from given valueAtStart
    float percentChange = (variedAverage - valueAtStart) / valueAtStart;
    return percentChange;
}

