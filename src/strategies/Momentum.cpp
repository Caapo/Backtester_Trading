#include "strategies/Momentum.hpp"
#include "strategies/Indicators.hpp"
#include <algorithm>
#include <iostream>

namespace bt{

Momentum::Momentum(const std::string& name_) : IStrategy(name_) {}



void Momentum::onStart(){
    std::cout << "\nStarting Momentum strategy\n";
    std::cout << "\nCas classique en trading : Periode = 15 - 20\n";
    std::cout << "Entrez la periode (10 - 30) : "; //Nombre de bougies prises en compte pour les calculs
    int period_tmp=-1;
    while(!verifCin(period_tmp, 10, 30)){
        std::cout << "Entrez la periode (10 - 30) : ";
    }
    period = period_tmp;
}







std::optional<OrderRequest> Momentum::onCandle(const Candle& c, const Portfolio& p, std::string assetName){
    lastCloses.push_back(c.getClose());

    if(lastCloses.size() <= period){
        std::cout << "Not enough data to execute Momentum strategy\n";
        return std::nullopt;
    }

    /*Determiner le momentum*/
    double mom = MomentumIDC(lastCloses, period);

    /*Trigger pour ne pas declencer des ordres a la chaine si un meme signal est toujours en cours*/
    bool prevTrigger = trigger;
    trigger = (mom > 0);


    double range = *std::max_element(lastCloses.end()-period, lastCloses.end()) - 
    *std::min_element(lastCloses.end()-period, lastCloses.end());

    double minSL = 0.3 * range;
    double marge = 0.2 * range;
    OrderRequest oreq;

    /*Buy : Momentum positif*/
    if(mom > 0 && !prevTrigger){
        double lastLow = *std::min_element(lastCloses.end() - period, lastCloses.end());
        oreq.date = c.getDate();
        oreq.asset = assetName;
        oreq.side = Side::BUY;
        oreq.entryPrice = c.getClose();
        oreq.stopLoss = std::min(lastLow - marge, c.getClose() - minSL);
        oreq.takeProfit = c.getClose() + p.getRRR() * (c.getClose() - oreq.stopLoss);
        oreq.quantity = 0.0;
        return oreq;
    }

    /*Sell : Momentum negatif*/
    if(mom < 0 && prevTrigger){
        double lastHigh = *std::max_element(lastCloses.end() - period, lastCloses.end());
        oreq.date = c.getDate();
        oreq.asset = assetName;
        oreq.side = Side::SELL;
        oreq.entryPrice = c.getClose();
        oreq.stopLoss = std::max(lastHigh + marge, c.getClose() + minSL);
        oreq.takeProfit = c.getClose() - p.getRRR() * (oreq.stopLoss - c.getClose());
        oreq.quantity = 0.0;
        return oreq;
    }

    return std::nullopt;
}



void Momentum::onEnd(){
    std::cout << "Stopping Momentum strategy\n";
}


} //end namespace bt
