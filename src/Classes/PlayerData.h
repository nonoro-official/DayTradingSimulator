    //
    // Created by sonik on 7/24/2025.
    //

    #ifndef PLAYERDATA_H
    #define PLAYERDATA_H
    #include "Company.h"
    #include "Stock.h"
    #include <vector>

    class PlayerData {
    public:
        // Access the singleton instance
        static PlayerData& Instance() {
            static PlayerData instance; // Guaranteed to be initialized only once
            return instance;
        }

        float cash;
        int weekExecutionDelay;

        // Upgrade effects
        int showPredictionTier = 0;   // Enables trend hints
        int weekExecutionDelayTier = 0;   // Increases transaction turn-around
        float sellBonusMultiplier = 1.0f; // No bonus by default

        PlayerData();

        void AddStock(Stock* stock);
        std::vector<Stock*>& GetStocks();
        float GetTotalPortfolioValue(); // total value of all stocks
        float GetTotalProfitLoss(Company* company, float avgBuyPrice);

        void UpgradePrediction();
        void UpgradeWeekExecution();

        std::string GetMarketPrediction(Company* company);

    private:
        std::vector<Stock*> stocks = std::vector<Stock*>();
    };

    #endif //PLAYERDATA_H
