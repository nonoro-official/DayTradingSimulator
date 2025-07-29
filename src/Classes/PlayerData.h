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
        bool showPrediction = false;   // Enables trend hints
        int graphZoom = 1;             // Graph zoom level

        PlayerData();

        void AddStock(Stock* stock);
        std::vector<Stock*>& GetStocks();
        float GetTotalPortfolioValue(); // total value of all stocks
        float GetTotalProfitLoss(Company* company, float avgBuyPrice);

    private:
        std::vector<Stock*> stocks = std::vector<Stock*>();
    };

    #endif //PLAYERDATA_H
