#ifndef _Portfolio_hpp_
#define _Portfolio_hpp_
#include <vector>
#include <string>
#include <optional>

#include "core/Candle.hpp"
#include "core/Position.hpp"
#include "core/Order.hpp"
#include "utils.hpp"


namespace bt{

/*---------------------------------------- class Portfolio ---------------------------------------*/
/*
Le portfolio est la piece centrale du trader.
Il gere le capital, calcul l'exposition au marche pour une potentielle position et autorise ou non les executions 
des ordres donnes par les strategies en verifiant la coherence des ordres.
*/

class Portfolio{

friend std::ostream& operator<<(std::ostream &os, const Portfolio &p);

private:
    double capital{0.0};
    double margin{0.0}; /*Fonds disponible pour s'exposer au marche*/
    double marginUsed{0.0}; /*Fonds engagees pour maintenir l'exposition au marche*/
    double riskPourcentage{0.0}; /*Pourcentage du capital mis a risque pour chaque ordre*/
    double RRR{0.0}; /*Ratio Risk Reward minimal */
    double riskAmount{0.0}; 
    double leverage = 0.0;
    std::vector<Position> positions; /*Info sur les positions en cours*/
    std::vector<Order> ordersHistory; 
    std::vector<Position> positionsHistory; 



public:    
    Portfolio(double capital_, double riskPourcentage_, double RRR_, double leverage_);
    Portfolio() = default;
    // Portfolio(const Portfolio&) = default;
    // Portfolio(Portfolio&&) = default;

    // Portfolio& operator=(const Portfolio&) = default;
    // Portfolio& operator=(Portfolio&&) = default;

    // ~Portfolio() = default;


    /*Methodes*/
    std::optional<Order> prepareOrder(OrderRequest &oreq);
    int executeOrder(Order &order);
    StatusO updatePositionsVect(const Order &o);
    int updatePortfolio(double pnlRealized_, double marginFreed_, double positionValue_);
    CloseResult refreshAllPos(const Candle &c); //actualise les positions en cours
    int winningTrades() const;
    int losingTrades() const;
    void closingPosition(int id, const Candle &c);
    void closeAllPos(const Candle &c);
    void showPos(int id);
    void showAllPos();
    void printPortfolio() const;
    void reset(){ capital = 0; margin = 0; positions.clear(); ordersHistory.clear(); }

    /*Getters/Setters*/
    double getRRR() const { return RRR; }    
    double getLeverage() const { return leverage; }
    double getCapital() const { return capital; }
    double getMargin() const { return margin; }
    void setLeverage(double leverage_) { if(leverage_ >= 0.0) leverage = leverage_; }
    // Position getPosition(const std::string &asset) const;
    const std::vector<Position>& getPositionsVect() const { return positions; }
    const std::vector<Position>& getPositionsHistory() const { return positionsHistory; }
    const std::vector<Order>& getOrdersHistory() const { return ordersHistory; }


    


};


} // end namespace bt

#endif //_Portfolio_hpp_

