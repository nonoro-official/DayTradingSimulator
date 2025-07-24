#include "GenerateRandomMarket.h"

GenerateRandomMarket::GenerateRandomMarket(int points, float amplitude, float frequency)
    : amountOfPoints(points), amplitude(amplitude), frequency(frequency) {}

void GenerateRandomMarket::InitializeMarket() {
    const siv::PerlinNoise::seed_type seedVal = this->seed;
    const siv::PerlinNoise perlin(seedVal);

    generatedPoints.clear();
    generatedPoints.reserve(amountOfPoints);

    for (int i = 0; i < amountOfPoints; ++i) {
        float noise = perlin.noise1D_01(i * frequency); // 0–1 range
        GraphPoint point;
        point.position = {0, noise * amplitude};
        point.nextPoint = nullptr;
        generatedPoints.push_back(point);
    }

    // Link points if you want a chain
    for (int i = 0; i < amountOfPoints - 1; ++i) {
        generatedPoints[i].nextPoint = &generatedPoints[i + 1];
    }

    if (OnFinishInitialize) OnFinishInitialize();
}

const std::vector<GraphPoint>& GenerateRandomMarket::GetMarketValues() const {
    return generatedPoints;
}
