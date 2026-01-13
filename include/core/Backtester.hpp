#ifndef _BACKTESTER_HPP_
#define _BACKTESTER_HPP_

#include "core/MarketData.hpp"
#include "core/Portfolio.hpp"
#include "strategies/IStrategy.hpp"
#include "strategies/RandomStrategy.hpp"
#include "strategies/SMA.hpp"
#include "strategies/Bollinger.hpp"
#include "strategies/Momentum.hpp"
#include "core/Asset.hpp"
#include <memory>



namespace bt{



/*---------------------------------------- class Backtester ---------------------------------------*/
/*
Le backtester est le coeur du projet de backtesting. Il orchestre toute la simulation en manipulant 
les autres classes.
*/

class Backtester {

    private:
        MarketData market;
        Portfolio portfolio;
        std::vector<Asset> assets;
        std::vector<std::unique_ptr<IStrategy>> strategies;
        std::string strategyInUse;

        bool stopRequest{false};
        int print{-1};
        int idClosePos{-1};
        int closeAll{-1};
        int idShowPos{-1};
        int autoTrade{1}; 
        int executeTrade{-1};
        int speedLevel{2};
        bool waitInput = false;

    public:
        Backtester(const std::string &path, const std::string &asset, double capital_, double riskP_, double RRR,
        double leverage);
        Backtester(double capital_, double riskP_, double RRR, double leverage);
        Backtester() = default;
        
        /*Methodes*/
        void printA();
        size_t selectA();
        void printS();
        size_t selectS();
        void run();
        void stopSimulation(){stopRequest = true;}
        void listenCommands();
        std::ostream& printPortfolio(std::ostream& os) const {os << portfolio << "\n"; return os;}

        /*Getters/Setters*/
        bool getStopRequest() const {return stopRequest;}
        int getWinningTrades() const {return portfolio.winningTrades();}
        int getLosingTrades() const {return portfolio.losingTrades();}
        std::string getStrategyInUse() const {return strategyInUse;}
        // double getPnlRealized() const {return portfolio.getPnlRealized();}
        // void showPositionsHistory() const {return portfolio.showPositionsHistory();}

        const std::string& getAsset() const {return market.getAsset();}



        

};


} //end namespace bt

#endif