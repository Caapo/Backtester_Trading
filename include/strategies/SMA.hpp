#ifndef _SMA_HPP_
#define _SMA_HPP_
#include "strategies/IStrategy.hpp"
#include "utils.hpp"
#include "core/Portfolio.hpp"


namespace bt{

/*---------------------------------- class SMA *----------------------------------*/
/*
Cette strategie repose sur le croisement de 2 moyennes mobiles simple (SMA) et de periode differentes (1 courte, 1 longue).
La SMA courte est plus reactive.
La SMA longue est plus representative de la tendance globale.
Lorsque la SMA courte passe au dessus de la SMA longue, un signal d'achat est detecte (Potentil debut de hausse)
Lorsque la SMA courte passe en dessous de la SMA longue, un signal de vente est detecte (Ralentissement/Retournement)

*/


class SMA: public IStrategy {
    private:
        size_t sizeMax{0}; //SMA 1
        size_t sizeMin{0}; //SMA 2

        std::vector<double> lastCloses;
        int idx{0};


    public:
        SMA(const std::string &name_);
        SMA() = default;

        void onStart() override;
        void stockCandle(const Candle &c);
        std::optional<OrderRequest> onCandle(const Candle &c, const Portfolio &p, std::string assetName) override;
        void onEnd() override;
};


}

#endif