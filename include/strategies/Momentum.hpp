#ifndef _MOMENTUM_HPP_
#define _MOMENTUM_HPP_

#include "strategies/IStrategy.hpp"
#include <vector>

namespace bt{


/*--------------------------------------------- class Momentum --------------------------------------------- */
/*
Le momentum est calcule a partir de la difference entre le prix courant et le prix des 'period' bougies precedentes. 
Un momentum positif indique une dynamique haussiere (BUY).
Un momentum negatif indique une dynamique baissiere (SELL).
*/


class Momentum : public IStrategy{
private:
    size_t period{0};
    std::vector<double> lastCloses;

public:
    Momentum(const std::string& name_);

    void onStart() override;
    std::optional<OrderRequest> onCandle(const Candle& c, const Portfolio& p, std::string assetName) override;
    void onEnd() override;
};

}

#endif
