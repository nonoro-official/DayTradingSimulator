
#ifndef GENERATERANDOMMARKET_H
#define GENERATERANDOMMARKET_H

#include <vector>
#include <functional>
#include "Graph.h"
#include "../Libraries/PerlinNoise.hpp"

class GenerateRandomMarket {
public:
    int seed;
    float amplitude, frequency, smoothness;
    float currentValue = 0.5f; // 0 - 1

    Vector2 eventCooldownRange = {2, 6};
    float eventChance = .25f;

    enum MarketState {
        Normal, TrendUp, TrendDown, Hold, Volatile, Cooldown
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
    Vector2 trendTimeRange = {4, 16};
    Vector2 randomTrendStrength = {-.025, .025};

    // Holds
    NoiseType holdNoiseType = WhiteNoise;
    float holdNoiseMultiplier = .2f;
    Vector2 holdTimeRange = {4, 16};

    // Volatile
    NoiseType volatileNoiseType = WhiteNoise;
    float volatileNoiseMultiplier = 1.25f;
    Vector2 volatileTimeRange = {4, 16};

    GenerateRandomMarket(float amplitude, float frequency);

    void InitializeMarket();
    void SetNormalValues(NoiseType noiseType, float noiseMultiplier);
    void SetTrendValues(NoiseType noiseType, float noiseMultiplier, Vector2 timeRange, Vector2 trendStrengthRange);
    void SetHoldValues(NoiseType noiseType, float noiseMultiplier, Vector2 timeRange);
    void SetVolatileValues(NoiseType noiseType, float noiseMultiplier, Vector2 timeRange);

    const std::vector<GraphPoint*> &GetMarketValues() const;
    std::function<void()> OnFinishInitialize = nullptr;

    GraphPoint* GenerateNextPoint();

private:
    float time = 0.0f;
    std::vector<GraphPoint*> generatedPoints;
    siv::PerlinNoise perlinNoise = siv::PerlinNoise(0); // default seed
    float edgePadding = .1f;

    void SetMarketState(MarketState state);
};

#endif //GENERATERANDOMMARKET_H