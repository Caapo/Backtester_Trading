#include "core/Position.hpp"
#include <iostream>
#include <iomanip>



namespace bt{


Position::Position() : id{-1} {}
Position::Position(int id_, const std::string &asset_, double quantity_, double entryPrice_, const std::string &openDate_, 
double stopLoss_, double takeProfit_, StatusP status_) : 
id{id_}, asset{asset_}, quantity{quantity_}, entryPrice{entryPrice_}, exitPrice{0.0}, openDate{openDate_},
stopLoss{stopLoss_}, takeProfit{takeProfit_}, status{status_} {} 


/*-------------------------------------------- update ---------------------------------------*/
/*
Cette methode : 
- Verifie si un TP ou un SL est atteint et appelle close pour cloturer la position le cas echeant
*/

CloseResult Position::update(const Candle &c, double leverage){ 
    /*Protection*/
    if(this->status != StatusP::LONG && this->status != StatusP::SHORT){
        throw std::invalid_argument("[DEBUG Position::update] Invalid side provided\n");
    }

    CloseResult result;
    double currentMarketPrice = c.getClose();
    double closedQuantity{this->quantity};
    
    
    /*Cas d'un TP atteint*/
    if(((this->status == StatusP::LONG) && (currentMarketPrice >= this->takeProfit)) || 
    ((this->status == StatusP::SHORT) && (currentMarketPrice <= this->takeProfit))){
            // this->quantity = 0.0;
            std::cout << "\n<<<<< POSITION " << this->id << " HAS BEEN CLOSED AT " << this->takeProfit <<
            " (TP WAS TOUCHED) >>>>>" << std::endl;
            return close(c.getDate(), this->takeProfit, closedQuantity, leverage);
    }
    /*Cas d'un SL atteint*/
    else if(((this->status == StatusP::LONG) && (currentMarketPrice <= this->stopLoss)) || 
    ((this->status == StatusP::SHORT) && (currentMarketPrice >= this->stopLoss))){
            // this->quantity = 0.0;
            std::cout << "\n <<<<< POSITION " << this->id << " HAS BEEN CLOSED AT " << this->stopLoss <<
            " (SL WAS TOUCHED) >>>>>" << std::endl;
            return close(c.getDate(), this->stopLoss, closedQuantity, leverage);
    
    }

    // std::cout << *this;

    return result;

}


/*------------------------------ close ------------------------------*/
/*
Cette methode : 
- cloture une position et renvoie une structure avec les informations necessaires (PnL, closeDate...)
*/
CloseResult Position::close(const std::string &closeDate_, double exitPrice, double closedQuantity, double leverage){

    CloseResult result;
    result.closedQuantity = closedQuantity;
    this->quantity -= closedQuantity;

    /*Calcul du pnlRealized*/
    double entryPricePos = this->getEntryPrice();
    if (this->getStatus() == StatusP::LONG){
        result.pnlRealized = (exitPrice - entryPricePos) * closedQuantity;
        this->pnlRealized += result.pnlRealized;
        
    }
    else if (this->getStatus() == StatusP::SHORT){
        result.pnlRealized = (entryPricePos - exitPrice) * closedQuantity;
        this->pnlRealized += result.pnlRealized;
        
    }
    
    /*Recuperation de la margin consommee*/
    double marginFreedTmp = closedQuantity * this->entryPrice; 
    result.marginFreed = marginFreedTmp / leverage;
    
    /*Mise a jour du status de la position*/
    if(this->quantity <= 0.0){
        this->pnlUnrealized = 0.0;
        this->exitPrice = exitPrice;
        this->quantity = closedQuantity;
        this->closeDate = closeDate_;
        this->status = StatusP::CLOSED;
    }


    return result;

}

/*------------------------------ refreshPnlUnrealized ------------------------------*/
/*
Cette methode :
- Actualise le PnL non realisee d'une position encore en cours pour suivre son evolution au cours de la simulation
*/
void Position::refreshPnlUnrealized(const Candle &c){
  
    double currentMarketPrice = c.getClose();
    switch(this->status){

        case StatusP::CLOSED : this->pnlUnrealized = 0.0; break;
        case StatusP::LONG : this->pnlUnrealized = (currentMarketPrice - this->entryPrice) * this->quantity; break;
        case StatusP::SHORT : this->pnlUnrealized = (this->entryPrice - currentMarketPrice) * this->quantity; break;
        case StatusP::NONE : std::cout << "[DEBUG] Position::refreshPnlU Position status is not valid\n";

    }

}



void Position::reset(){
    asset = "";
    quantity = 0.0;
    entryPrice = 0.0;
    exitPrice = 0.0;
    openDate = "";
    closeDate = "";
    pnlRealized = 0.0;
    pnlUnrealized = 0.0;
    stopLoss = 0.0;
    takeProfit = 0.0;
    status = StatusP::NONE;
}



/*
Cette surcharge d'operateur :
- Affiche une position
*/
std::ostream& operator<<(std::ostream &os, const Position &p){

    os << "\nPosition Informations\n"
    << "ID: " << p.id
    << " | Asset: " << p.asset
    << " | OpenDate: " << p.openDate 
    << " | CloseDate: " << p.closeDate
    << "\n" << "EntryPrice: " << p.entryPrice
    << " | ExitPrice: " << p.exitPrice
    << " | Quantity: " << p.quantity 
    << "\n" << "pnlRealized: " << p.pnlRealized 
    << " | pnlUnrealized: " << p.pnlUnrealized
    << " | Status: " << to_string(p.status)
    << "\n" << "SL: " << p.stopLoss
    << " | TP: " << p.takeProfit
    << "\n" ;

    return os;
}

}


