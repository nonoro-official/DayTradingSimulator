#ifndef GENERATERANDOMMARKET_H
#define GENERATERANDOMMARKET_H

#include <vector>
#include <functional>
#include "Graph.h"
#include "../Libraries/PerlinNoise.hpp"

class GenerateRandomMarket {
public:
    int seed;
    int amountOfPoints;
    float amplitude, frequency, smoothness;

    GenerateRandomMarket(int points, float amplitude, float frequency, float smoothness = 0.5f);

    void InitializeMarket();
    const std::vector<GraphPoint>& GetMarketValues() const;
    std::function<void()> OnFinishInitialize = nullptr;

private:
    float time = 0.0f;
    std::vector<GraphPoint> generatedPoints;
};

#endif //GENERATERANDOMMARKET_H
