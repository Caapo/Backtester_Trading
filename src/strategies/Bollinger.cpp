#include "strategies/Bollinger.hpp"
#include "strategies/Indicators.hpp"
#include <iostream>

namespace bt {

Bollinger::Bollinger(const std::string& name_) : IStrategy(name_) {}



void Bollinger::onStart(){
    std::cout << "\nStarting Bollinger Bands strategy\n";
    std::cout << "\nCas classique en trading : Periode = 15 - 20  | Multiplicateur = 2\n";
    std::cout << "Entrez la periode (10 - 30): ";
    int period_tmp=-1;
    while(!verifCin(period_tmp, 10, 30)){
        std::cout << "Entrez la periode (10 - 30) : ";
    }
    period = period_tmp;

    while(k <= 0 || k > 3){
        std::cout << "Entrez le Multiplicateur (k) : ";
        std::cin >> k;
        verifCin(k);
    }
}




std::optional<OrderRequest> Bollinger::onCandle(const Candle& c, const Portfolio& p, std::string assetName){
    lastCloses.push_back(c.getClose());

    if(lastCloses.size() < period){
        std::cout << "Not enough data to execute Bollinger strategy\n";
        return std::nullopt;
    }

    /*Calcul moyenne mobile simple et ecart type*/
    double sma = bt::SMAIDC(lastCloses, period);
    double stddev = bt::StdDevIDC(lastCloses, period);

    /*Calcul des bandes upper et lower*/
    double upper = sma + k * stddev;
    double lower = sma - k * stddev;

    double price = c.getClose();

    bool prevTrigger = trigger;
    trigger = (price < lower || price > upper);

    double minSL = 1.2 * stddev; //ajuste le SL par rapport au timeframe (volatilite != sur les UT)
    double marge = 0.3 * stddev; //marge d'ecart servant a l'invalidation de la strategie en fonction de la volatilite


    OrderRequest oreq;


    /*BUY : cassure bande basse*/
    if(price < lower && !prevTrigger){
        oreq.date = c.getDate();
        oreq.asset = assetName;
        oreq.side = Side::BUY;
        oreq.entryPrice = price;
        oreq.stopLoss = std::min(lower - marge, price - minSL);
        oreq.takeProfit = price + p.getRRR() * (price - oreq.stopLoss);
        oreq.quantity = 0.0;
        return oreq;
    }

    /*SELL : cassure bande haute*/
    if(price > upper && !prevTrigger){
        oreq.date = c.getDate();
        oreq.asset = assetName;
        oreq.side = Side::SELL;
        oreq.entryPrice = price;
        oreq.stopLoss = std::max(upper + marge, price + minSL);
        oreq.takeProfit = price - p.getRRR() * (oreq.stopLoss - price);
        oreq.quantity = 0.0;
        return oreq;
    }

    //reset du trigger si on revient dans les bandes
    if(price > lower && price < upper){
        trigger = false;
    }

    return std::nullopt;
}



void Bollinger::onEnd(){
    std::cout << "Stopping Bollinger strategy\n";
}

} //end namespace bt
