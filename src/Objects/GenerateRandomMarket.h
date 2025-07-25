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
    float volatility = 0.0025f;  // e.g., 0.5 = moderate noise, 2.0 = high chaos
    Vector2 trendStrengthRange = { -0.003f, 0.008f }; // Min and max trend strength
    float trendChangeChance = 0.3f; // Chance of changing trend per step
    Vector2 trendDurationRange = { 4, 18 };  // min and max trend duration in steps
    float trendMaintainChance = .8f; // 0.0 to 1.0: 50% chance to continue current trend

    float holdChance = 0.4f; // 10% chance to enter a hold state
    Vector2 holdDurationRange = { 4, 16 }; // How long the market holds (steps)
    float holdNoiseDampening = .5f; // 5% of full amplitude during hold periods


    GenerateRandomMarket(float amplitude, float frequency);

    void InitializeMarket();
    const std::vector<GraphPoint>& GetMarketValues() const;
    std::function<void()> OnFinishInitialize = nullptr;

    GraphPoint* GenerateNextPoint();

private:
    float time = 0.0f;
    std::vector<GraphPoint> generatedPoints;
    siv::PerlinNoise perlinNoise = siv::PerlinNoise(0); // default seed
    float activeTrendStrength = 0.0f;
    float activeTrendDuration = 0.0f;
    int holdStepsRemaining = 0;           // How many steps left to hold
};

#endif //GENERATERANDOMMARKET_H
