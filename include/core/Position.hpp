#ifndef _Position_hpp_
#define _Position_hpp_
#include "core/Order.hpp"
#include "core/Candle.hpp"
#include "utils.hpp"



namespace bt{

/*-------------------- struct CloseResult --------------------*/
/*
Fournit les informations sur la cloture d'une position
*/
struct CloseResult{
    double closedQuantity{0.0};
    double pnlRealized{0.0};
    double marginFreed{0.0};
};


/*-------------------- class Position --------------------*/
/*
Une Position est la consequence d'un ordre execute.
Elle impacte directement le portfolio par ses gains ou pertes engendrees lors de sa fermeture.
Elle est cloturee lorsque son stopLoss ou son takeProfit est atteint ou si l'utilisateur la cloture 
manuellement.
*/



class Position {

friend std::ostream& operator<<(std::ostream &os, const Position &p);

private: 
    int id;
    std::string asset{""};
    double quantity{0.0};
    double entryPrice{0.0};
    double exitPrice{0.0}; //NEW
    std::string openDate{""};
    std::string closeDate{""};
    double pnlRealized{0.0};
    double pnlUnrealized{0.0};
    double stopLoss{0.0};
    double takeProfit{0.0};
    StatusP status{StatusP::NONE};

public:
    

    Position(int id_, const std::string &asset_, double quantity_, double entryPrice_, const std::string &openDate_, 
    double stopLoss_, double takeProfit_, StatusP status_);
    Position();

    // Position(const Position&) = default;
    // Position(Position&&) = default;

    // Position& operator=(const Position&) = default;
    // Position& operator=(Position&&) = default;
    
    // ~Position() = default;


    /*Methodes*/
    CloseResult update(const Candle &c, double leverage);
    CloseResult close(const std::string &closeDate_, double exitPrice, double closedQuantity, double leverage);
    void refreshPnlUnrealized(const Candle &c);
    void reset();


    /*Getters/Setters*/
    int getID() const { return id; }
    double getEntryPrice() const { return entryPrice; }
    double getQuantity() const { return quantity; }
    double getPnlRealized() const { return pnlRealized; }
    double getPnlUnrealized() const { return pnlUnrealized; }
    const std::string &getAsset() const { return asset; }
    StatusP getStatus() const { return status; }


};



} //end namespace bt





#endif