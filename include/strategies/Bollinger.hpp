#ifndef _BOLLINGER_HPP_
#define _BOLLINGER_HPP_

#include "strategies/IStrategy.hpp"
#include <vector>

namespace bt {


/*------------------------------------ class Bollinger ------------------------------------ */
/*
Les bandes de bollinger sont 2 bandes (1 superieur et 1 inferieur).
Lorsque l'on passe la bande superieur, l'actif est considere en surachat (SELL).
Lorsque l'on passe la bande inferieur, l'actif est considere en survente (BUY).
Elles sont calculees a partir d'une moyenne mobile simple (sma), d'un multiplicateur (k) 
et d'un ecart type (stddev).
*/

class Bollinger : public IStrategy {
private:
    size_t period{0};
    double k{-1.0};
    std::vector<double> lastCloses;

public:
    Bollinger(const std::string& name_);

    void onStart() override;
    std::optional<OrderRequest> onCandle(const Candle& c, const Portfolio& p, std::string assetName) override;
    void onEnd() override;
};

} //end namespace bt

#endif
