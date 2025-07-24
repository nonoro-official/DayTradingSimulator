#include "GenerateRandomMarket.h"

#include <iostream>
#include <bits/ostream.tcc>

GenerateRandomMarket::GenerateRandomMarket(float amplitude, float frequency)
    : amplitude(amplitude), frequency(frequency) {}

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
    float noise = perlinNoise.noise1D_01(time * frequency);  // 0–1 range
    float value = noise * amplitude;

    GraphPoint *newPoint = new GraphPoint();
    newPoint->position = { time, value };
    newPoint->prevPoint = nullptr;

    if (!generatedPoints.empty()) {
        newPoint->prevPoint = &generatedPoints.back();
    }

    generatedPoints.push_back(*newPoint);

    time += 0.5f;  // Or a smaller delta for smoother simulation

    return newPoint;
}