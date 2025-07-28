#include "GenerateRandomMarket.h"

#include <chrono>
#include <iostream>
#include <bits/ostream.tcc>
#include <cstdlib>  // for rand()
#include <ctime>    // for time()

#include "raymath.h"


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

GraphPoint* GenerateRandomMarket::GenerateNextPoint() {
    if (timeInState <= 0) currentState = Normal;

    switch (currentState) {
        case Normal:

            break;
    }

    float yValue = currentValue;
    /*float noiseValue = perlinNoise.noise1D(amplitude * time * frequency) * perlinNoiseMultiplier;
    */

    float noiseValue = Remap(static_cast<float>(rand()) / RAND_MAX, 0, 1, -.5, .5) * defaultNoiseMultiplier;
    yValue += yValue * noiseValue;

    // Random Trend
    float chance = static_cast<float>(rand()) / RAND_MAX;
    if (chance >= .5f) currentValue -= randomTrendStrength.y;

    // Clamp to minmax
    currentValue = Clamp(currentValue, 0.025, 1.0f);
    yValue = Clamp(yValue, 0.025f, 1.0f);

    std::cout << std::endl << "y " << yValue << std::endl;
    std::cout << "n " << noiseValue << std::endl;


    GraphPoint* newPoint = new GraphPoint(yValue);
    newPoint->prevPoint = nullptr;

    generatedPoints.push_back(newPoint);

    time += 1;
    return newPoint;
}