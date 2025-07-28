
#ifndef GENERATERANDOMMARKET_H
#define GENERATERANDOMMARKET_H

#include <vector>
#include <functional>
#include "Graph.h"
#include "../Libraries/PerlinNoise.hpp"
#include "Classes/MarketData.h"

class GenerateRandomMarket {
public:
    int seed;
    float amplitude, frequency, smoothness;
    float currentValue = 0.5f; // 0 - 1

    enum MarketState {
        Normal, TrendUp, TrendDown, Hold, Volatile
    };

    enum NoiseType {
        PerlinNoise, WhiteNoise
    };

    MarketState currentState = Normal;

    int timeInState = 0;

    // Normal
    NoiseType defaultNoiseType = WhiteNoise;
    float defaultNoiseMultiplier = .6f; // use 80% of noise

    // Trends
    NoiseType trendNoiseType = WhiteNoise;
    float trendNoiseMultiplier = .4f;
    Vector2 trendCooldownRange = {4, 16};
    Vector2 trendTimeRange = {4, 16};
    Vector2 randomTrendStrength = {-.025, .025};

    // Holds
    NoiseType holdNoiseType = WhiteNoise;
    float holdNoiseMultiplier = .2f;
    Vector2 holdCooldownRange = {4, 16};
    Vector2 holdTimeRange = {4, 16};

    // Volatile
    NoiseType volatileNoiseType = WhiteNoise;
    float volatileNoiseMultiplier = 1.0f;

    GenerateRandomMarket(float amplitude, float frequency);

    void InitializeMarket();
    const std::vector<GraphPoint*> &GetMarketValues() const;
    std::function<void()> OnFinishInitialize = nullptr;

    GraphPoint* GenerateNextPoint();

private:
    float time = 0.0f;
    std::vector<GraphPoint*> generatedPoints;
    siv::PerlinNoise perlinNoise = siv::PerlinNoise(0); // default seed
    float activeTrendStrength = 0.0f;
    float activeTrendDuration = 0.0f;
    int holdStepsRemaining = 0;           // How many steps left to hold
};

#endif //GENERATERANDOMMARKET_H