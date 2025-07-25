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
    float volatility = 0.05f;  // e.g., 0.5 = moderate noise, 2.0 = high chaos
    float trendStrength = 0.0f;  // How strongly the market trends up/down (e.g., -0.1 to +0.1)
    float trendChangeChance = 0.025f; // Chance of changing trend per step
    float trendDuration = 1.0f; // How long the current trend lasts

    GenerateRandomMarket(float amplitude, float frequency);

    void InitializeMarket();
    const std::vector<GraphPoint>& GetMarketValues() const;
    std::function<void()> OnFinishInitialize = nullptr;

    GraphPoint* GenerateNextPoint();

private:
    float time = 0.0f;
    std::vector<GraphPoint> generatedPoints;
    siv::PerlinNoise perlinNoise = siv::PerlinNoise(0); // default seed
};

#endif //GENERATERANDOMMARKET_H
