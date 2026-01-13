#ifndef _RandomStrategy_hpp_
#define _RandomStrategy_hpp_
#include "strategies/IStrategy.hpp"
#include "utils.hpp"




namespace bt{

/*----------------------------- class RandomS -----------------------------*/
/*
Strategie aleatoire base sur aucun indicateurs.
Sert de test pour le backtester et de point de comparaison pour les autres strategies
plus techniques. 
*/

class RandomS : public IStrategy {

    public:
        
        RandomS(const std::string &name_) : IStrategy(name_) {}
        RandomS() = default;
    
        // RandomS(const RandomS&) = default;   
        // RandomS(RandomS&&) = default;       

        // RandomS& operator=(const RandomS&) = default; 
        // RandomS& operator=(RandomS&&) = default; 

        // ~RandomS() override = default; 

        /*Methodes*/
        void onStart() override;
        std::optional<OrderRequest> onCandle(const Candle &c, const Portfolio &p, std::string assetName) override;
        void onEnd() override;
};

}

#endif