#include "strategies/SMA.hpp"
#include "strategies/Indicators.hpp"
#include <optional>
#include <iostream>
#include <numeric> 


namespace bt{


SMA::SMA(const std::string &name_) : IStrategy{name_}, sizeMax{0}, sizeMin{0}{}

void SMA::onStart(){
    std::cout << "\nStarting SMA (SimpleMovingAverage) Strategy...\n";
    std::cout << "\nCas classique en trading : Courte = 10, Longue = 20\n";
    std::cout << "Entrez votre Moyenne Mobile courte (10 - 29) : ";
    int sizeMin_tmp=-1;
    while(!verifCin(sizeMin_tmp, 10, 29)){
        std::cout << "Entrez votre Moyenne Mobile courte (10 - 29) : ";
    }
    sizeMin = sizeMin_tmp;

    std::cout << "Entrez votre Moyenne mobile longue (10 - 30 && > Courte) : ";
    int sizeMax_tmp=-1;
    while(!verifCin(sizeMax_tmp, sizeMin+1, 30)){
        std::cout << "Entrez votre Moyenne mobile longue (10 - 30 && > Courte) : ";
    }
    sizeMax = sizeMax_tmp;
    
}



std::optional<OrderRequest> SMA::onCandle(const Candle &c, const Portfolio &p, std::string assetName){

    
    printf("SMA(ID=%d) Working on Candle...\n", id);
    lastCloses.push_back(c.getClose());
    OrderRequest oreq;

    if(lastCloses.size() < sizeMax){
        std::cout << "Not enough data to execute SMA strategy\n";
        return std::nullopt;  
    }

    /*Calcul des moyennes grace a l'indicateurs template*/
    double smaMin = SMAIDC<double>(lastCloses, sizeMin);
    double smaMax = SMAIDC<double>(lastCloses, sizeMax);

    /*Detecter le nouveau croisement*/
    bool prevCross = trigger;
    trigger = (smaMin > smaMax);

    double buffer = 0.5 * std::abs(smaMin - smaMax);
    double marge = 0.25 * abs(smaMin - smaMax);



    /*Buy : smaMin (courte) passe au-dessus de smaMax (longue) (&& smaMin precedemment en dessous)*/
    if(trigger && !prevCross){
        std::cout << "BUY SIGNAL detected by SMA\n";
        oreq.date       = c.getDate();
        oreq.asset      = assetName;
        oreq.side       = Side::BUY;
        oreq.entryPrice = c.getClose();
        oreq.stopLoss   = std::min(smaMax - marge, c.getClose() - buffer);
        oreq.takeProfit = c.getClose() + p.getRRR() * (c.getClose() - oreq.stopLoss);
        oreq.quantity   = 0.0; // sera calculee par le portefeuille
        return oreq;
    }

    /*Sell : smaMin (courte) passe en dessous de smaMax (longue) (&& smaMin precedemment au dessus)*/
    else if(!trigger && prevCross){
        std::cout << "SELL SIGNAL detected by SMA\n";
        oreq.date       = c.getDate();
        oreq.asset      = assetName;
        oreq.side       = Side::SELL;
        oreq.entryPrice = c.getClose();
        oreq.stopLoss   = std::max(smaMin + marge, c.getClose() + buffer);
        oreq.takeProfit = c.getClose() - p.getRRR() * (oreq.stopLoss - c.getClose());
        oreq.quantity   = 0.0; // sera calculee par le portefeuille
        return oreq;
    }


    
    std::cout << "NO SIGNAL detected by SMA\n";
    return std::nullopt; 



}



void SMA::onEnd(){
    std::cout << "Stopping SMA Strategy...\n";
}



}


