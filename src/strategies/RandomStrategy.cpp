#include "strategies/RandomStrategy.hpp"
#include "core/Candle.hpp"
#include "core/MarketData.hpp"
#include "core/Portfolio.hpp"
#include <ctime>
#include <iostream>
#include <cstdlib>


namespace bt{


void RandomS::onStart(){
    std::cout << "Starting RandomS Strategy...\n";
}
                


std::optional<OrderRequest> RandomS::onCandle(const Candle &c, const Portfolio &p, std::string assetName){

    /*Trigger aleatoire pour savoir si un ordre doit etre genere*/
    trigger = (rand() % 2 == 0);

    if (!trigger) {
        std::cout << "No signal detected by RandomS\n";
        return std::nullopt;
    }

    std::cout << "RandomS(ID=" << id << ") Working on Candle...\n";

    Side side = (rand()%2 == 0) ? Side::BUY : Side::SELL;
    double entryPrice = c.getClose();

    /*Generation d'un SL aleatoire*/
    double minSlDelta = entryPrice * 0.001; 
    double slDelta = entryPrice * (0.005 + ((double)rand() / RAND_MAX) * 0.01);
    if(slDelta < minSlDelta) 
        slDelta = minSlDelta;

    double stopLoss = (side == Side::BUY) ? entryPrice - slDelta : entryPrice + slDelta;

    double RRR = p.getRRR();
    double takeProfit = (side == Side::BUY) ? entryPrice + slDelta*RRR : entryPrice - slDelta*RRR;

    /*Requete d'ordre*/
    OrderRequest req;
    req.date       = c.getDate();
    req.asset      = assetName;
    req.side       = side;
    req.entryPrice = entryPrice;
    req.stopLoss   = stopLoss;
    req.takeProfit = takeProfit;
    req.quantity   = 0.0; // sera calculee par le portefeuille

    return req;
}



void RandomS::onEnd() {
    std::cout << "Stopping RandomS Strategy...\n";
}


} //end namespace bt