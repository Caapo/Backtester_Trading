#include <iostream>
#include "core/Order.hpp"



namespace bt{

/*----------------- Attributs statiques -----------------*/
int Order::orderNb{0};



OrderRequest::OrderRequest(int id_, const std::string &date_, const std::string &asset_, Side side_, double entryPrice_, 
double RRR_, double quantity_, double stopLoss_, double takeProfit_) 
: id(id_), date(date_), asset(asset_), side(side_), entryPrice(entryPrice_), RRR(RRR_), quantity(quantity_),
stopLoss(stopLoss_), takeProfit(takeProfit_) {}

Order::Order() : id{orderNb++} {}
Order::Order(const std::string &date_, const std::string &asset_, Side side_, double entryPrice_, double quantity_,  StatusO status_) : 
id{orderNb++}, date{date_}, asset{asset_}, side{side_}, entryPrice{entryPrice_}, quantity{quantity_}, status{status_} {}



/*----------------- Methode setStatus -----------------*/
/*
Met a jour le status d'un ordre
*/

void Order::setStatus(const StatusO &status_){
    if(this->status == StatusO::NONE) this->status = status_;
    else if(this->status == StatusO::OPEN){
        if(status_ == StatusO::EXECUTED || status_ == StatusO::CANCELLED) this->status = status_;
    }
}



/*----------------- Methode setTakeProfit -----------------*/
/*
Met a jour le takeProfit d'un ordre (Seuil fermant la position en gain si atteint)
*/

bool Order::setTakeProfit(double takeProfit_){
    if(this->side == Side::NONE){
        std::cout << "[ENGINE] Error, your order does not have any status.\n";
        return false;
    }

    if(this->side == Side::BUY){
        if(takeProfit_ <= this->entryPrice){
            std::cout << "[ENGINE] LONG : Your takeprofit cannot be lower or equal to your entry price.\n";
            return false;
        }
        this->takeProfit = takeProfit_;
        return true;
    }
    
    if(this->side == Side::SELL){
        if(takeProfit_ >= this->entryPrice){
            std::cout << "[ENGINE] SHORT : Your takeprofit cannot be greater or equal to your entry price.\n";
            return false;
        }
        this->takeProfit = takeProfit_;
        return true;
    }

    std::cout << "[DEBUG Order::setTakeProfit] No conditions has been executed\n";
    return false;
}




/*----------------- Methode setStopLoss -----------------*/
/*
Met a jour le takeProfit d'un ordre (Seuil fermant la position en perte si atteint)
*/

bool Order::setStopLoss(double stopLoss_){
    

    if(this->side == Side::NONE){
        std::cout << "[ENGINE] Error, your order does not have any status.\n";
        return false;
    }

    if(this->side == Side::BUY){
        if(stopLoss_ >= this->entryPrice){
            std::cout << "[ENGINE] LONG : Your stoploss cannot be greater or equal to your entry price.\n";
            return false;
        }
        this->stopLoss = stopLoss_;
        return true;
    }
    
    if(this->side == Side::SELL){
        if(stopLoss_ <= this->entryPrice){
            std::cout << "[ENGINE] SHORT : Your stoploss cannot be lower or equal to your entry price.\n";
            return false;
        }
        this->stopLoss = stopLoss_;
        return true;
    }

    std::cout << "[DEBUG Order::setStopLoss] No conditions has been executed\n";
    return false;
} 



/*----------------- surcharge d'operateur << -----------------*/
/*
Pour l'affichage d'un ordre
*/

std::ostream& operator<<(std::ostream &os, const Order &o){
    os << "\nOrder Informations\n"
    << "ID: " << o.id 
    << " | Date: " << o.date 
    << " | Asset: " << o.asset 
    << " | Side: " << to_string(o.side)
    << "\n" << "EntryPrice: " << o.entryPrice
    << " | Quantity: " << o.quantity 
    << " | StopLoss: " << o.stopLoss 
    << " | TakeProfit: " << o.takeProfit 
    << " | Status: " << to_string(o.status)
    << "\n";

    return os;
}

}//end namespace bt