#ifndef _MarketData_hpp_
#define _MarketData_hpp_
#include <vector>
#include "core/Candle.hpp"
#include <iostream>



namespace bt{

/*---------------------------------------- class MarketData ---------------------------------------*/
/*
Represente le graphique de l'actif selectionne grace aux donnees du csv que l'on transmet aux bougies/chandeliers.
*/

class MarketData { 

    private :
        std::vector<Candle> candles;
        std::string assetName;
        size_t currentIdx = 0;

    public :        
        MarketData(const std::string &pathname, const std::string &asset);
        MarketData() = default;

        // MarketData(const MarketData&) = default;
        // MarketData(MarketData&&) = default;

        // MarketData& operator=(const MarketData&) = default;
        // MarketData& operator=(MarketData&&) = default;

        // ~MarketData() = default;

        /*Methodes*/
        int loadCSV(const std::string &path, const std::string &assetName);
        bool hasNext() const;
        const Candle& next();
        void reset();

        /*Getters/Setters*/
        const std::vector<Candle>& getCandles() const { return candles; }
        double getLastPrice() const { return candles[currentIdx].getClose(); }
        size_t getCurrentIdx() const { return currentIdx; }
        size_t getDataSize() const { return candles.size(); }
        const std::string& getAsset () const { return assetName; }




};




} //end namespace bt

#endif