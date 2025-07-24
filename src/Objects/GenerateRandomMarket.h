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
